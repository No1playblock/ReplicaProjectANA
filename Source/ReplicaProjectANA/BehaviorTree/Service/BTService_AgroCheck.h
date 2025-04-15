// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AgroCheck.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UBTService_AgroCheck : public UBTService
{
	GENERATED_BODY()
	
private:	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


	UPROPERTY(EditAnywhere)
	float Radius;
};
