// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueDataBase.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FDialogueData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = Dialogue)
	FString ConversationID;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	FString ID;
	
	UPROPERTY(EditAnywhere, Category = Dialogue)
	FString Speaker;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	FString Participants;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	FString Text;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	FString ChoiceText;

	UPROPERTY(EditAnywhere, Category = Dialogue)
	FString NextID;

};
