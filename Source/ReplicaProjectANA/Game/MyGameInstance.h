// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameData/SaveLoadClass.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void Save();

	UFUNCTION(BlueprintCallable)
	void Load();

private:
	FString SlotName = TEXT("save1");

	AANACharacter* player;
	USaveLoadClass* SystemInstance;
};
