// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_IsAttackToFalse.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UBTTask_IsAttackToFalse : public UBTTaskNode
{
	GENERATED_BODY()
	


	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	struct FBlackboardKeySelector IsAttacked;

};
