// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Notifies/PaperZDAnimNotify.h"
#include "AN_AnimationEnd.generated.h"


/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API UAN_AnimationEnd : public UPaperZDAnimNotify
{
	GENERATED_BODY()

	virtual void OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance = nullptr) const override;
};
