// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Components/BoxComponent.h"

#include "SlicedObject.generated.h"

UCLASS()
class REPLICAPROJECTANA_API ASlicedObject : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Sliced();
private:
	
	ASlicedObject();
	virtual void BeginPlay() override;


	

	void SetComponent();

	void PickRandomObject();
	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprite", meta= (AllowPrivateAccess = "true"))
	TArray<UPaperSprite*> TopSpriteArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
	TArray<UPaperSprite*> BottomSpriteArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperSpriteComponent> TopSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprite", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperSpriteComponent> BottomSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> BoxComponent;	

private:
	
	bool bIsSliced;
	int32 RandomIndex;
};
