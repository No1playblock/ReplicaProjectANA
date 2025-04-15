// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskTailSwing.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UBTTaskTailSwing : public UBTTaskNode
{
	GENERATED_BODY()
  
private:
    UBTTaskTailSwing();
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UFUNCTION()
    void ForloopWithDelay(UBehaviorTreeComponent* OwnerComp);

    UFUNCTION()
    void TailSwing(int32 Index);

    UFUNCTION()
    void FinishTask(UBehaviorTreeComponent* OwnerComp);


    UPROPERTY(EditAnywhere, Category = "Blackboard")
    struct FBlackboardKeySelector IsTaskingKey;

    UPROPERTY(EditAnywhere)
    int32 NumPoints;

    UPROPERTY(EditAnywhere)
    float TailRadius;

    UPROPERTY(EditAnywhere)
    float CircleRadius;

    UPROPERTY(EditAnywhere)
    float ReverseTailDelay = 0.53f;

    UPROPERTY(EditAnywhere)
    float TailSwingTime = 0.5f;

    UPROPERTY(EditAnywhere)
    float ReverseTailSwingTime = 0.8f;

    UPROPERTY(EditAnywhere)
    float Damage=0.0f;

    FTimerHandle TailSwingTimerHandle;

    FVector Center;

    TArray<FVector> AttackPoints;
};
