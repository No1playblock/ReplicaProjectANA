// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ChoiceButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UChoiceButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ChoiceButton)
	{
		ChoiceButton->OnClicked.AddDynamic(this, &UChoiceButtonWidget::HandleClicked);
	}
}

void UChoiceButtonWidget::SetupChoice(const FString& Text, const FString& InID)
{
	ChoiceID = InID;
	if (ChoiceText)
	{
		ChoiceText->SetText(FText::FromString(Text));
	}
}

void UChoiceButtonWidget::HandleClicked()
{
	OnChoiceClicked.Broadcast(ChoiceID);
}