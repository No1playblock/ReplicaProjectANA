// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagableCharacter.h"

// Add default functionality here for any IDamagableCharacter functions that are not pure virtual.

void IDamagableCharacter::TakeDamage(int32 DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Damage On Interface"));
}
