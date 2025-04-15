// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsAttackToFalse.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/Mob_AIController.h"

//키값을 못가져올때가 있어서 이걸 써줘야함.
void UBTTask_IsAttackToFalse::InitializeFromAsset(UBehaviorTree& Asset)
{
    Super::InitializeFromAsset(Asset);
    UBlackboardData* BBAsset = GetBlackboardAsset();
    if (BBAsset)
    {
        IsAttacked.ResolveSelectedKey(*BBAsset);
    }
    else
    {
        UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initizlize task : %s"), *(GetName()));
    }
}

EBTNodeResult::Type UBTTask_IsAttackToFalse::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AMob_AIController* ThisController = Cast<AMob_AIController>(OwnerComp.GetAIOwner());

    if (ThisController)
    {
        ThisController->GetBlackboardComp()->SetValueAsBool(IsAttacked.SelectedKeyName, false);
        return EBTNodeResult::Succeeded;
    }
    return EBTNodeResult::Failed;
}