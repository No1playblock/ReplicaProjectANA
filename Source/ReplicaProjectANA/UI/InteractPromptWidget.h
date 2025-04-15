// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "InteractPromptWidget.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UInteractPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetSubTitleText(const FString& NewText);

private:
	virtual void NativeConstruct() override;


	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UCanvasPanel> MainCanvas;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<class URichTextBlock> SubTitleText;

};
