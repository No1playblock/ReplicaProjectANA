// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueChoiceWidget.generated.h"

/**
 * 
 */
class UGridPanel;
class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceSelected, const FString&, ChoiceID);


UCLASS()
class REPLICAPROJECTANA_API UDialogueChoiceWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION()
	void InitChoices(const TArray<FString>& Choices, const TArray<FString>& ChoiceIDs);

	UPROPERTY()
	FOnChoiceSelected OnChoiceSelected;

private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleChoiceSelected(const FString& SelectedID);


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGridPanel> ChoiceGrid;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> ChoiceButton;

};
