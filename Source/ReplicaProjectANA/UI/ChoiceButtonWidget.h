// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ChoiceButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoiceClicked, const FString&, ChoiceID);

UCLASS()
class REPLICAPROJECTANA_API UChoiceButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// 외부에서 텍스트와 ID 설정
	void SetupChoice(const FString& Text, const FString& InID);

	// 버튼 클릭 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnChoiceClicked OnChoiceClicked;

private:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleClicked();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ChoiceButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ChoiceText;

	FString ChoiceID;

};
