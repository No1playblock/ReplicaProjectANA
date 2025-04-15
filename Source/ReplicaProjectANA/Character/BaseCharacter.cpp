// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

void ABaseCharacter::LaunchSetup(float Speed, float Duration, float CoolDownTime, FVector Direction)
//���� Speed, Duration, CollDownTime�� �����ͼ� �˹��Ŵ.
//������ ���� ���� �����Ͽ� ���ݿ� ���� �˹��� �ٸ��� ��������
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

		// Launch�� �Է� �Ұ�
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
	// Launch ��Ÿ���� ����� �� �����.
}

bool ABaseCharacter::GetbIsLaunching()
{
	return bIsLaunching;
}
