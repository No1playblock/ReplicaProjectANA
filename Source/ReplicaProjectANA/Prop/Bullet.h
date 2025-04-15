// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSpriteComponent.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Bullet.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UPaperSpriteComponent;
class UNiagaraComponent;

UCLASS()
class REPLICAPROJECTANA_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintCallable)
	void Throw(FVector Vel);

private:
	ABullet();

	UFUNCTION()
	void OnOverlapBullet(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = Collision)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = PaperSprite)
	TObjectPtr<UPaperSpriteComponent> PaperSprite;

	UPROPERTY(EditAnywhere, Category = Niagara)
	TObjectPtr<UNiagaraComponent> BulletTrail;


};
