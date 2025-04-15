// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTaskMoveToPlayer.generated.h"



/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UBTTaskMoveToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
    

private:

    UBTTaskMoveToPlayer();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetToFollowKey;

    UPROPERTY(EditAnywhere, Category = "AI")
    float AcceptanceRadius = 100.f;

    UBehaviorTreeComponent* CachedOwnerComp = nullptr;

    FAIRequestID MoveRequestID;

};
