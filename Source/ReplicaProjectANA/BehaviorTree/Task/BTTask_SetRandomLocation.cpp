// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/Mob_AIController.h"
#include "Character/MobCharacter.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

EBTNodeResult::Type UBTTask_SetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMob_AIController* ThisAIController = Cast<AMob_AIController>(OwnerComp.GetAIOwner());
	if (!ThisAIController) 	return EBTNodeResult::Failed;
	
	AMobCharacter* ThisMobCharacter = Cast<AMobCharacter>(ThisAIController->GetPawn());
	if (!ThisMobCharacter) 	return EBTNodeResult::Failed;

	UNavigationSystemV1* Navsystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if(Navsystem==nullptr) return EBTNodeResult::Failed;

	FNavLocation RandomNavLocation;
	bool isRandomPoint = Navsystem->GetRandomPointInNavigableRadius(ThisMobCharacter->GetActorLocation(), Radius, RandomNavLocation, NULL);
	
	if (isRandomPoint)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(ThisAIController, RandomNavLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}