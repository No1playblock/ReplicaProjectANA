// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/SaveLoadClass.h"
#include "Character/ANACharacter.h"


void UMyGameInstance::Init()
{
	Super::Init();
	// 초기화 로직 추가
	if (GetWorld())
	{
		player = Cast<AANACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		UE_LOG(LogTemp, Warning, TEXT("GetWorld()"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() doesn't find"));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("GameInstance Initialized"));
}

void UMyGameInstance::Save()
{

	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("player doesn't find"));

		return;
	}

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))			//있는지 확인
	{
		UE_LOG(LogTemp, Warning, TEXT("Save"));
		if (!(IsValid(SystemInstance)))
		{
			SystemInstance = Cast<USaveLoadClass>(UGameplayStatics::CreateSaveGameObject(USaveLoadClass::StaticClass()));
		}

		SystemInstance->playerPos = player->GetActorTransform();
		SystemInstance->InAndOutDoorState = player->GetCharacterInAndOutState();
		UGameplayStatics::SaveGameToSlot(SystemInstance, SlotName, 0);
	}
}

void UMyGameInstance::Load()
{
	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("player doesn't find"));
		return;
	}
		

	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))			//있는지 확인
	{
		UE_LOG(LogTemp, Warning, TEXT("Load"));
		SystemInstance = Cast<USaveLoadClass>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		player->SetActorTransform(SystemInstance->playerPos);
		player->SetCharacterInAndOutState(SystemInstance->InAndOutDoorState);

	}
	else
	{
		if (!(IsValid(SystemInstance)))
		{
			SystemInstance = Cast<USaveLoadClass>(UGameplayStatics::CreateSaveGameObject(USaveLoadClass::StaticClass()));
		}

		SystemInstance->playerPos = player->GetActorTransform();
		SystemInstance->InAndOutDoorState = player->GetCharacterInAndOutState();
		UGameplayStatics::SaveGameToSlot(SystemInstance, SlotName, 0);
	}
}


