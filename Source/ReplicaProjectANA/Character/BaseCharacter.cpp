// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABaseCharacter::LaunchSetup(float Speed, float Duration, float CoolDownTime, FVector Direction)
//적의 Speed, Duration, CollDownTime을 가져와서 넉백시킴.
//공격의 따른 값을 변경하여 공격에 따른 넉백을 다르게 조정가능
{
	if (GetWorldTimerManager().IsTimerActive(LaunchCoolDownTimerHandle)) {
		return;
	}
	
	LaunchSpeed = Speed;
	LaunchDuration = Duration;
	LaunchCoolDownTime = CoolDownTime;
	LaunchDirection = Direction;
	
	LaunchStart();
}

void ABaseCharacter::LaunchStart()
{
	if (!bIsLaunching) {
		bIsLaunching = true;

		// Launch중 입력 불가
		// DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));

		GetWorldTimerManager().SetTimer(LaunchTimerHandle, this, &ABaseCharacter::LaunchStop, LaunchDuration, false);
	}
}

void ABaseCharacter::LaunchStop()
{
	bIsLaunching = false;

	// EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GetWorldTimerManager().SetTimer(LaunchCoolDownTimerHandle, this, &ABaseCharacter::LaunchCoolDown, LaunchCoolDownTime, false);
}

void ABaseCharacter::LaunchPerform()
{
	FVector LaunchVelocity = LaunchDirection * LaunchSpeed;
	GetCharacterMovement()->Velocity = LaunchVelocity;
}

void ABaseCharacter::LaunchCoolDown()
{
	// Launch 쿨타임이 종료된 후 실행됨.
}

bool ABaseCharacter::GetbIsLaunching()
{
	return bIsLaunching;
}
