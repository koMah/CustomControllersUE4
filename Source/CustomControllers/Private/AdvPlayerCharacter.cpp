// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomControllersPrivatePCH.h"

#include "CustomControllers.h"
#include "AdvPlayerCharacter.h"
#include "AdvPlayerController.h"
#include "UnrealNetwork.h"

AAdvPlayerCharacter::AAdvPlayerCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void AAdvPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAdvPlayerCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AAdvPlayerCharacter, ViewRotationDelta);
}

FRotator AAdvPlayerCharacter::GetViewRotation() const
{
	if (AAdvPlayerController* AdvPC = Cast<AAdvPlayerController>(Controller))
	{
		return AdvPC->GetViewRotation();
	}

	return GetActorRotation();
}