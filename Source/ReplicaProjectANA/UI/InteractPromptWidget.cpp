// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractPromptWidget.h"
#include "Interface/InteractionInterface.h"
#include "Components/CanvasPanel.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

void UInteractPromptWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!SubTitleText)
	{
		UE_LOG(LogTemp, Error, TEXT("SubTitleText is null in NativeConstruct! Check widget binding."));
	}
}

void UInteractPromptWidget::SetSubTitleText(const FString& NewText)
{

	if (!SubTitleText)
	{
		UE_LOG(LogTemp, Warning, TEXT("SubTitleText is null"));
		return;
	}

	SubTitleText->SetText(FText::FromString(NewText));
	

}