// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "Interface/InteractionInterface.h"
#include "SoulFlower.generated.h"

UCLASS()
class REPLICAPROJECTANA_API ASoulFlower : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	

private:
	ASoulFlower();

	virtual void Interact() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UPaperSpriteComponent> SoulFlower;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UInteractionComponent> InteractionComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MapWidget;

	TObjectPtr<UUserWidget> WidgetInstance;

};
