// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/WidgetComponent.h"
#include "Interface/InteractionInterface.h"
#include "InteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REPLICAPROJECTANA_API UInteractionComponent : public UWidgetComponent
{
	GENERATED_BODY()


private:
	UInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

	void PressE();

	void ShowInteractionPrompt(bool bShow);

private:
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> InteractContext;

	UPROPERTY()
	TObjectPtr<class UInputAction> InteractAction;

	UPROPERTY(EditAnywhere)
	float DetectionRadius;

	float DistanceToPlayer;

	void SetWidgetTransform();

	UUserWidget* InteractionWidgetInstance;

	APlayerController* PlayerController;
	APawn* PlayerPawn;
		
};
