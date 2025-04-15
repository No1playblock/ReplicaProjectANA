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
	// �ܺο��� �ؽ�Ʈ�� ID ����
	void SetupChoice(const FString& Text, const FString& InID);

	// ��ư Ŭ�� ��������Ʈ
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
