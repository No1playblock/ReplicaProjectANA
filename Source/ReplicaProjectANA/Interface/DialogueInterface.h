// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogueInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class REPLICAPROJECTANA_API IDialogueInterface
{
	GENERATED_BODY()

public:
	virtual void Dialogue(FString SubtitleString)=0;
};
