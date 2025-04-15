// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "TextBallonWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UTextBallonWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetWidgetTransform();
	
private:
	APlayerController* PlayerController;

};
