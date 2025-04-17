// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTaskSmashGround.h"
#include "AIController.h"
#include "Character/MobCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/ANACharacter.h"

UBTTaskSmashGround::UBTTaskSmashGround()
{
	NodeName = TEXT("TaskSmashGround");

	TargetToFollowKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskSmashGround, TargetToFollowKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTaskSmashGround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)	return EBTNodeResult::Failed;

	APawn* ControlledPawn = AIController->GetPawn();

	if (!ControlledPawn) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(!BlackboardComp) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;
	Mob = Cast<AMobCharacter>(ControlledPawn);
	Player = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetToFollowKey.SelectedKeyName));
	BossLocation = Mob->GetActorLocation();

	Mob->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	
	GetWorld()->GetTimerManager().SetTimer(FloatTimerHandle, this, &UBTTaskSmashGround::Floating, 0.01f, true);	//Tick처럼 1000될때까지 올려


	return EBTNodeResult::InProgress;
}

void UBTTaskSmashGround::Floating()
{
	if (Mob->GetActorLocation().Z <= BossLocation.Z + MaxHeight)		//도달할때까지
	{
		Mob->AddActorWorldOffset(FVector(0.0f, 0.0f, 5.0f));
	}
	else	//도달하면
	{
		GetWorld()->GetTimerManager().ClearTimer(FloatTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(FollowTimerHandle, this, &UBTTaskSmashGround::FollowPlayer, 0.01f, true);
		GetWorld()->GetTimerManager().SetTimer(SmashGroundTimerHandle, this, &UBTTaskSmashGround::FinishFollow, FollowTime, false);
	}
}

void UBTTaskSmashGround::FollowPlayer()
{
	
	
	FVector PlayerLocation = FVector(Player->GetActorLocation().X, Player->GetActorLocation().Y, 0.0f);
	FVector MobLocation = FVector(Mob->GetActorLocation().X, Mob->GetActorLocation().Y, 0.0f);

	Mob->AddActorWorldOffset((MoveSpeed*(PlayerLocation - MobLocation).GetSafeNormal(0.00001f)));
}

void UBTTaskSmashGround::FinishFollow()
{
	GetWorld()->GetTimerManager().ClearTimer(FollowTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(SmashGroundTimerHandle, this, &UBTTaskSmashGround::SmashGround, 0.01f, true);

}

void UBTTaskSmashGround::SmashGround()
{
	FHitResult HitResult;

	Mob->AddActorWorldOffset(FVector(0.0f, 0.0f, -100.0f), true, &HitResult);

	if (HitResult.bBlockingHit)
	{
		GetWorld()->GetTimerManager().ClearTimer(SmashGroundTimerHandle);
		for (int i = 1; i <= 3; i++)
		{
			FTimerHandle ShockWaveTimerHandle;

			GetWorld()->GetTimerManager().SetTimer(ShockWaveTimerHandle, FTimerDelegate::CreateLambda([this, i]()
				{
					ShockWave(i);
				}), ShockWaveDelayTime* (i-1)+0.01f, false);
		}
		FTimerHandle FinishTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(FinishTimerHandle, this, &UBTTaskSmashGround::FinishTask, ShockWaveDelayTime * 2.0f, false);
	}

}

void UBTTaskSmashGround::ShockWave(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("ShockWave : %d"), Index);
	for (int i = 0; i < ShockWaveNum; i++)
	{
		FRotator Rotate = FRotator(0.0f, (360 / ShockWaveNum) * i, 0.0f);
		//로테이션의 전방벡터 * 충격파의 박스크기 * 울린 횟수로 위치정하기
		FVector ShockWaveLocation = Rotate.Vector()*(2*ShockWaveBoxHalfSize.X) * Index;		//생성될 박스위치	
		FVector MobLocation = FVector(Mob->GetActorLocation().X, Mob->GetActorLocation().Y, 200.0f);

		FVector ShockWaveCenterLocation = ShockWaveLocation + MobLocation;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));
		
		FHitResult OutHit;

		bool bHit = UKismetSystemLibrary::BoxTraceSingleForObjects(
			GetWorld(),
			ShockWaveCenterLocation,
			ShockWaveCenterLocation,
			ShockWaveBoxHalfSize,
			Rotate,
			ObjectTypes,
			false,                     // TraceComplex
			{},
			EDrawDebugTrace::ForDuration, // Debug Type
			OutHit,
			true,                      // Ignore Self
			FLinearColor::Blue,         // Trace Color
			FLinearColor::Red,       // Hit Color
			0.4f                       // Draw Time
		);
		
		if (OutHit.GetActor()->StaticClass() == AANACharacter::StaticClass())
		{
			Cast<AANACharacter>(OutHit.GetActor())->TakeDamage(Damage);
		}
	}
}

void UBTTaskSmashGround::FinishTask()
{
	FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);

}
