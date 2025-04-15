// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecCloseEnough.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UBTDecCloseEnough : public UBTDecorator
{
	GENERATED_BODY()
	


private:

    UBTDecCloseEnough();
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

    UPROPERTY(EditAnywhere, Category = "Condition")
    float AcceptableDistance = 200.f;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector TargetToFollowKey;

    
};
