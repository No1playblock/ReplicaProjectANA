// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "BaseCharacter.h"
#include "Interface/DamagableCharacter.h"
#include "MobCharacter.generated.h"

/**
 * 
 */
UCLASS()
class REPLICAPROJECTANA_API AMobCharacter : public ABaseCharacter, public IDamagableCharacter
{
	GENERATED_BODY()

protected:

	FVector GetKnockBackDirection();

	void HitStopEnd();

	void Tick(float DeltaTime);

	


private:
	int32 MaxHealth;
	int32 CurrentHealth;

	FTimerHandle HitStopTimerHandle;
	float HitStopDuration;

	bool bIsAttacking=false;

public:

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(int32 DamageAmount) override;

	FORCEINLINE bool GetbIsAttacking() const { return bIsAttacking; }
	FORCEINLINE void SetbIsAttacking(bool NewbIsAttacking) { bIsAttacking = NewbIsAttacking; }
	AMobCharacter();

	UPROPERTY(EditAnywhere, Category = Pawn)
	class UBehaviorTree* BehaviorTree;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Mob_isAttack=false;
};
