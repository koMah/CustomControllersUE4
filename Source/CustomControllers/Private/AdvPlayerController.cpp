// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomControllersPrivatePCH.h"

#include "CustomControllers.h"
#include "AdvPlayerController.h"
#include "UnrealNetwork.h"

AAdvPlayerController::AAdvPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CanLookAround = true;
	FreeLookState = FreeLook::Unactive;
	LockControlRotationInFreeLook = true;
	ViewRotationResetSpeed = 10.0f;
	MaxFreeLookYaw = 85.0f;
	bReplicates = true;
}

void AAdvPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AAdvPlayerController::UpdateRotation(float DeltaTime)
{
	// Calculate Delta to be applied on Controllers Rotation
	FRotator DeltaRot(RotationInput);

	// View & Aim are now separate.
	// The controller rotation represents where we are aiming.
	// NewViewRotation represents where we are looking.
	
	if (FreeLookState == FreeLook::Pending)
	{
		if (!ViewIsAligned()){
			ResetViewRotation(DeltaTime, ViewRotationResetSpeed);
		}
		else {
			DisableFreeLook();
		}
		//return;
	}

	FRotator NewAimRotation = GetControlRotation();
	FRotator NewViewRotation = GetViewRotation();

	if (CanLookAround)
	{
		if (FreeLookState == FreeLook::Active)
		{
			PlayerCameraManager->ProcessViewRotation(DeltaTime, NewViewRotation, DeltaRot);
			// Limit how far we can turn our head before our torso begins to turn too.
			const float YawDif = FRotator::NormalizeAxis(NewAimRotation.Yaw - NewViewRotation.Yaw);

			if (!LockControlRotationInFreeLook) {
				float AddDeltaYaw = 0.0f;

				if (YawDif > MaxFreeLookYaw)
				{
					AddDeltaYaw = YawDif - MaxFreeLookYaw;
				}
				else if (YawDif < -MaxFreeLookYaw)
				{
					AddDeltaYaw = YawDif + MaxFreeLookYaw;
				}

				NewAimRotation.Yaw -= AddDeltaYaw;
			}

			NewViewRotation.Yaw = FMath::ClampAngle(NewViewRotation.Yaw, NewAimRotation.Yaw - MaxFreeLookYaw, NewAimRotation.Yaw + MaxFreeLookYaw);
		}
		else if (FreeLookState == FreeLook::Unactive)
		{
			PlayerCameraManager->ProcessViewRotation(DeltaTime, NewAimRotation, DeltaRot);
			NewViewRotation = NewAimRotation;
		}
	
	}
	else {
		PlayerCameraManager->ProcessViewRotation(DeltaTime, NewAimRotation, DeltaRot);
		NewViewRotation = NewAimRotation;
	}

	SetControlRotation(NewAimRotation);

	// If our view and aim are different, call SetViewRotation().
	if (NewViewRotation != NewAimRotation)
	{
		SetViewRotation(NewViewRotation);
	}

	APawn* const Pawn = GetPawnOrSpectator();
	if (Pawn)
	{
		Pawn->FaceRotation(NewAimRotation, DeltaTime);
	}

}

void AAdvPlayerController::ResetViewRotation(float DeltaTime, float Speed)
{
	FRotator NewRotation = FMath::RInterpTo(GetViewRotation(), GetControlRotation(), DeltaTime, Speed);
	SetViewRotation(NewRotation);
}

void AAdvPlayerController::SetControlRotation(const FRotator& NewRotation)
{
	ControlRotation = NewRotation;

	// Anything that is overriding view rotation will need to 
	// call SetViewRotation() after SetControlRotation().
	if (FreeLookState != FreeLook::Pending){
		SetViewRotation(NewRotation);
	}

	if (RootComponent && RootComponent->bAbsoluteRotation)
	{
		RootComponent->SetWorldRotation(GetControlRotation());
	}
}


void AAdvPlayerController::SetViewRotation(const FRotator& NewRotation)
{
	ViewRotation = NewRotation;
}


void AAdvPlayerController::EnableFreeLook()
{
	ChangeFreeLookState(FreeLook::Active);
}

void AAdvPlayerController::DisableFreeLook()
{
	ChangeFreeLookState(FreeLook::Pending);

	if (ViewIsAligned()){
		ChangeFreeLookState(FreeLook::Unactive);
	}
}

void AAdvPlayerController::PendingFreeLook()
{
	ChangeFreeLookState(FreeLook::Pending);
}

FRotator AAdvPlayerController::GetViewRotation() const
{
	return ViewRotation;
}

FRotator AAdvPlayerController::GetViewRotationDelta()
{
	FRotator Delta = GetControlRotation() - GetViewRotation();
	Delta.Normalize();
	return Delta;
}

bool AAdvPlayerController::ViewIsAligned()
{
	float CurrentViewYaw = GetViewRotation().Yaw < 0 ? GetViewRotation().Yaw + 360 : GetViewRotation().Yaw;
	return GetViewRotation().Equals(GetControlRotation(), 1.0f);
}

void AAdvPlayerController::ChangeFreeLookState(FreeLook::State State)
{
    FreeLookState = State;
	this->OnFreeLookChange(State);
}