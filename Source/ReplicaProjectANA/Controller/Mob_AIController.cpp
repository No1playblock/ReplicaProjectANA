// Fill out your copyright notice in the Description page of Project Settings.


#include "Mob_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"	
#include "Character/MobCharacter.h"

AMob_AIController::AMob_AIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	check(BlackboardComp);

	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	check(BehaviorTreeComp);

}

void AMob_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMobCharacter* possessedPawn = Cast<AMobCharacter>(InPawn);

	if (InPawn != nullptr)
	{
		BlackboardComp->InitializeBlackboard(*(possessedPawn->BehaviorTree->BlackboardAsset));
		
		BehaviorTreeComp->StartTree(*(possessedPawn->BehaviorTree));
		GetBlackboardComp()->SetValueAsVector(TEXT("HomeLocation"), InPawn->GetActorLocation());
	}

}
