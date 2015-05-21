// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AdvPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMCONTROLLERS_API AAdvPlayerCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Pawn")
	FRotator ViewRotationDelta;

	virtual void Tick(float DeltaSeconds) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "FreVIEW")
	virtual FRotator GetViewRotation() const override;
	
	
};
