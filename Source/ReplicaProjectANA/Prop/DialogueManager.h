// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameData/DialogueDataBase.h"

#include "DialogueManager.generated.h"




UCLASS()
class REPLICAPROJECTANA_API ADialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	void StartTalking(FString Speaker, FString ConvID);
	bool ProgressingDialogue();

private:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnChoiceButtonSelected(const FString& SelectedID);

	void SearchDialogue(FString Speaker, FString DialogueName);
	void ShowChoiceWidget(const TArray<FString>& Choices, const TArray<FString>& ChoiceIDs);
	void DialogueEnd();
	AActor* FindTaggedActor(UWorld* World, FName Tag);
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "DialogueDB")
	TObjectPtr<class UDataTable> BaseDialogueDT;
	
	UPROPERTY()
	TArray<AActor*> ParicipantActors;

	FDialogueData* Result;

	TArray<FDialogueData*> AllRows;

	FString SaveNextID;
	FString SaveConvID;
	
};
