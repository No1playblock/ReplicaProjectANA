// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Spitting.h"
#include "Character/MobCharacter.h"
#include "Character/ANACharacter.h"
#include "PaperFlipbookComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Mob_AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// 생성자에서 필터등록
UBTTask_Spitting::UBTTask_Spitting()
{
	TargetToFollow.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Spitting, TargetToFollow), AActor::StaticClass());
	IsTasking.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_Spitting, IsTasking));
}

void UBTTask_Spitting::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		TargetToFollow.ResolveSelectedKey(*BBAsset);
		IsTasking.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initizlize task : %s"), *(GetName()));
	}
}

EBTNodeResult::Type UBTTask_Spitting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMob_AIController* ThisController = Cast<AMob_AIController>(OwnerComp.GetAIOwner());

	AMobCharacter* ThisCharacter = Cast<AMobCharacter>(ThisController->GetPawn());

	if (!ThisController || ThisCharacter) return EBTNodeResult::Failed;

	FVector playerLocation = ThisCharacter->GetSprite()->GetSocketLocation("Mouth");

	AANACharacter* TargetToFollowActor = Cast<AANACharacter>(ThisController->GetBlackboardComp()->GetValueAsObject(TargetToFollow.SelectedKeyName));



	if (IsValid(TargetToFollowActor))
	{
		ThisCharacter->Mob_isAttack = true;
		FVector ForceDirection = (TargetToFollowActor->GetActorLocation() - playerLocation).GetSafeNormal();
		FVector ForceVector = ForceDirection * Force;

		ThisCharacter->GetCharacterMovement()->Velocity = ForceDirection * 50.0f;		//해당방향으로 캐릭터가 바라보게 하기위해 Velocity를 적용

		FActorSpawnParameters SpawnParams;

		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BPBullet, playerLocation, FRotator(0.f, 0.f, 0.f), SpawnParams);

		Bullet->Throw(ForceVector);

		ThisController->GetBlackboardComp()->SetValueAsBool(IsTasking.SelectedKeyName, false);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IsNOTValid"));

		return EBTNodeResult::Failed;
	}

}
