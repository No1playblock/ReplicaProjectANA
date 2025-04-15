// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulFlower.h"
#include "UI/InteractionComponent.h"
#include "UI/MapWidget.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ASoulFlower::ASoulFlower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SoulFlower = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
	
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	SoulFlower->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	SoulFlower->SetupAttachment(RootComponent);
	
	InteractionComponent->SetupAttachment(RootComponent);
}

void ASoulFlower::Interact()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	UE_LOG(LogTemp, Warning, TEXT("SoulFlower"));
	if (WidgetInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Delete"));
		WidgetInstance->RemoveFromParent();
		WidgetInstance = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Create"));
		WidgetInstance = CreateWidget<UUserWidget>(PC, MapWidget);
		if (WidgetInstance)
		{
			WidgetInstance->AddToViewport();
		}
	}
	
}


