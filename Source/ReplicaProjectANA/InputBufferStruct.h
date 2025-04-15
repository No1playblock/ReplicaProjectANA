// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputBufferStruct.generated.h"
/**
 * 
 */
class REPLICAPROJECTANA_API InputBufferStruct
{
public:
	InputBufferStruct();
	~InputBufferStruct();
};

UENUM(BlueprintType)
enum class FInputBufferType : uint8 {
	NONE UMETA(Display = "None"),
	ATTACK UMETA(Display = "Attack"),
	DASH UMETA(Display = "Dash")
};

USTRUCT(BlueprintType)
struct FInputBuffer
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsValid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInputBufferType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolDownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LaunchDirection;
};

