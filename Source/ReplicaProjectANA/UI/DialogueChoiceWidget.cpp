// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DialogueChoiceWidget.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/ChoiceButtonWidget.h"

void UDialogueChoiceWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// 위젯이 생성되었을 때 호출됨
}

void UDialogueChoiceWidget::InitChoices(const TArray<FString>& Choices, const TArray<FString>& ChoiceIDs)
{
	if (!ChoiceGrid) return;

	ChoiceGrid->ClearChildren();

	for (int32 i = 0; i < Choices.Num(); ++i)
	{

		UChoiceButtonWidget* ChoiceButtonWidget = CreateWidget<UChoiceButtonWidget>(GetWorld(), ChoiceButton);
		if (!ChoiceButtonWidget) continue;

		ChoiceButtonWidget->SetupChoice(Choices[i], ChoiceIDs.IsValidIndex(i) ? ChoiceIDs[i] : TEXT(""));

		ChoiceButtonWidget->OnChoiceClicked.AddDynamic(this, &UDialogueChoiceWidget::HandleChoiceSelected);

		int32 Row = i / 2;
		int32 Col = i % 2;

		ChoiceGrid->AddChildToGrid(ChoiceButtonWidget, Row, Col);
	}
}

void UDialogueChoiceWidget::HandleChoiceSelected(const FString& SelectedID)
{
	//버튼이 클릭되면 호출
	OnChoiceSelected.Broadcast(SelectedID);
}
