// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Character/ANACharacter.h"
#include "SaveLoadClass.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API USaveLoadClass : public USaveGame
{
	GENERATED_BODY()

	
public:

	UPROPERTY()
	FTransform playerPos;

	UPROPERTY()
	ECharacterState InAndOutDoorState;

};
