// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "BaseCharacter.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API ABaseCharacter : public APaperZDCharacter
{
	GENERATED_BODY()
	
protected:
	void LaunchSetup(float Speed, float Duration, float CoolDownTime, FVector Direction);
	void LaunchStart();
	void LaunchStop();
	void LaunchPerform();
	virtual void LaunchCoolDown();

	bool GetbIsLaunching();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Launch", meta = (AllowPrivateAccess = "true"))
	FTimerHandle LaunchCoolDownTimerHandle;

private:
	// 대쉬 관련 변수
	bool bIsLaunching;
	FTimerHandle LaunchTimerHandle;
	FVector LaunchDirection;
	float LaunchSpeed;
	float LaunchDuration;
	float LaunchCoolDownTime;
};
