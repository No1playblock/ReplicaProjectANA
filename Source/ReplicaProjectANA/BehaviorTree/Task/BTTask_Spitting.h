// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Mob_AIController.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Prop/Bullet.h"
#include "BTTask_Spitting.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UBTTask_Spitting : public UBTTaskNode
{
	GENERATED_BODY()
	
private:

	UBTTask_Spitting();

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	UPROPERTY(EditAnywhere, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> BPBullet;

	UPROPERTY(EditAnywhere, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector TargetToFollow;

	UPROPERTY(EditAnywhere, Category = "BlackBoard", meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector IsTasking;

	UPROPERTY(EditAnywhere, Category = "Force", meta = (AllowPrivateAccess = "true"))
	float Force;

	
};
