// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "AdvPlayerController.generated.h"


UENUM(BlueprintType)
namespace FreeLook
{
	//Max of 256 entries per type, each entry gets associated with 0-255
	enum State
	{
		Active 	UMETA(DisplayName = "Active"),
		Unactive UMETA(DisplayName = "Unactive"),
		Pending	UMETA(DisplayName = "Pending")
	};
}

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class CUSTOMCONTROLLERS_API AAdvPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()

public:
	virtual void UpdateRotation(float DeltaTime) override;
	
	virtual void SetControlRotation(const FRotator& NewRotation) override;

	/* View Rotation */
    void ChangeFreeLookState(FreeLook::State State);
	
	UFUNCTION(BlueprintCallable, Category = "Controller")
	void SetViewRotation(const FRotator& NewRotation);

	/**
	*  View & Movement direction are now separate.
	*  The controller rotation will determine which direction we will move.
	*  ViewRotation represents where we are looking.
	*/


	virtual void SetupInputComponent() override;

	void ResetViewRotation(float DeltaTime, float Speed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FreeLook")
	TEnumAsByte<FreeLook::State> FreeLookState;

	UPROPERTY(BlueprintReadOnly, Category = "FreeLook")
	FRotator ViewRotation;

	UPROPERTY(BlueprintReadOnly, Category = "FreeLook")
	FRotator ViewRotationDelta;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FreeLook")
	float ViewRotationResetSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FreeLook")
	bool CanLookAround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FreeLook")
	float MaxFreeLookYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FreeLook")
	bool LockControlRotationInFreeLook;
		
public:
	bool ViewIsAligned();
	
	UFUNCTION(BlueprintCallable, Category = "Controller|FreeLook")
	virtual void EnableFreeLook();

	UFUNCTION(BlueprintCallable, Category = "Controller|FreeLook")
	virtual void DisableFreeLook();

	UFUNCTION(BlueprintCallable, Category = "Controller|FreeLook")
	virtual void PendingFreeLook();
		
	UFUNCTION(BlueprintCallable, Category = "Controller|FreeLook")
	FRotator GetViewRotation() const;
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Controller")
	FRotator GetViewRotationDelta();

	/** Player's Health is Currently Above 50! Returns
	player's exact current health for convenience. This
	function runs every tick that the player's health
	is high enough! Play healthy sounds here. */
	UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "On FreeLook Change"))
	virtual void OnFreeLookChange(FreeLook::State CurrentFreeLookState);
};
