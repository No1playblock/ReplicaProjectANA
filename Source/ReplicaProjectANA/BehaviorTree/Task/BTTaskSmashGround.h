// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskSmashGround.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UBTTaskSmashGround : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTaskSmashGround();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void Floating();

	UFUNCTION()
	void FollowPlayer();

	UFUNCTION()
	void FinishFollow();

	UFUNCTION()
	void SmashGround();

	UFUNCTION()
	void ShockWave(int32 Index);

	UFUNCTION()
	void FinishTask();


	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector TargetToFollowKey;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 5.0f;

	UPROPERTY(EditAnywhere)
	float FollowTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float MaxHeight = 1000.0f;

	UPROPERTY(EditAnywhere)
	float ShockWaveDelayTime = 0.4f;

	UPROPERTY(EditAnywhere)
	int32 ShockWaveNum = 6;

	UPROPERTY(EditAnywhere)
	FVector ShockWaveBoxHalfSize = FVector(150.0f, 50.0f, 100.0f);

	UPROPERTY(EditAnywhere)
	float Damage;

	TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	TObjectPtr<AActor> Player;

	TObjectPtr<class AMobCharacter> Mob;
	FVector BossLocation;
	FTimerHandle FloatTimerHandle;
	FTimerHandle FollowTimerHandle;
	FTimerHandle SmashGroundTimerHandle;

};
