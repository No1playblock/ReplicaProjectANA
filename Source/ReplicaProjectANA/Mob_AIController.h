// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Mob_AIController.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API AMob_AIController : public AAIController
{
	GENERATED_BODY()

public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() { return BlackboardComp; }

private:
	AMob_AIController();
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	TObjectPtr<class UBlackboardComponent> BlackboardComp;

	UPROPERTY()
	TObjectPtr<class UBehaviorTreeComponent> BehaviorTreeComp;
	
};
