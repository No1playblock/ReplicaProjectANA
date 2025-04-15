// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Interface/InteractionInterface.h"
#include "Interface/DialogueInterface.h"
#include "GameData/DialogueDataBase.h"
#include "NPC.generated.h"

class UInteractionComponent;
/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API ANPC : public APaperZDCharacter, public IInteractionInterface, public IDialogueInterface
{
	GENERATED_BODY()

	

public:
	virtual void Dialogue(FString SubtitleString) override;

	void DialogueWidgetInvisible();

private:

	ANPC();
	virtual void BeginPlay() override;
	virtual void Interact() override;


	UPROPERTY()
	TObjectPtr<class ADialogueManager> DialogueMgr;

	UPROPERTY()
	TObjectPtr<class UInteractionComponent> InteractComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTextBallonWidgetComponent> TextBallonWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "DialogueDB")
	TObjectPtr<class UDataTable> BaseDialogueDT;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FString ConversationID;		//진행도에 따라 여러개로 해야할 경우 TArray로 수정하셈

	bool bTalking;

	int32 RowIndex = 1;

	TObjectPtr<class UInteractPromptWidget> DialogueWidget;

};
