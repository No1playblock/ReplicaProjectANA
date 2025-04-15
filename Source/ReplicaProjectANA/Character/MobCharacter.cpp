// Fill out your copyright notice in the Description page of Project Settings.


#include "MobCharacter.h"
#include "ANACharacter.h"
#include "Mob_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AMobCharacter::AMobCharacter() 
	:MaxHealth(100),
	CurrentHealth(100), 
	HitStopDuration(.5f)
{
	//BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	//check(BlackboardComp);
	
}

void AMobCharacter::TakeDamage(int32 DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Damage! Amount : %d"), DamageAmount);

	GetWorldTimerManager().SetTimer(HitStopTimerHandle, this, &AMobCharacter::HitStopEnd, HitStopDuration, false);
	//CustomTimeDilation = 0.0f;

	LaunchSetup(1000.0f, 0.2f, 0.2f, GetKnockBackDirection());
	FindComponentByClass<UCharacterMovementComponent>()->StopMovementImmediately();
	Cast<AMob_AIController>(GetController())->GetBlackboardComp()->SetValueAsBool(TEXT("IsAttacked"), true);
	//BlackboardComp->SetValueAsBool(TEXT("IsAttacked"), true);

	CurrentHealth -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("%f"), CurrentHealth);
}


FVector AMobCharacter::GetKnockBackDirection() {
	AActor* player;
	player = UGameplayStatics::GetActorOfClass(GetWorld(), AANACharacter::StaticClass());
	
	FVector Direction = (GetActorLocation() - player->GetActorLocation());

	return Direction.GetSafeNormal();
}

void AMobCharacter::HitStopEnd()
{
	CustomTimeDilation = 1.0f;
}

void AMobCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetbIsLaunching()) {
		LaunchPerform();
	}
}