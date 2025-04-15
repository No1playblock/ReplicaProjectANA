// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Character/ANACharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");
	
	
	
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	BulletTrail=CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	BulletTrail->SetupAttachment(CollisionComp);

	PaperSprite=CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSprite"));
	PaperSprite->SetupAttachment(CollisionComp);
	PaperSprite->SetCollisionProfileName("NoCollision");

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	//ProjectileMovement->InitialSpeed = 3000.f;
	//ProjectileMovement->MaxSpeed = 3000.f;
	//ProjectileMovement->bRotationFollowsVelocity = true;
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBullet);		// set up a notification for when this component hits something blocking
	
	InitialLifeSpan = 3.0f;
}
void ABullet::OnOverlapBullet(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor->IsA(AANACharacter::StaticClass()))
	{
		(Cast<AANACharacter>(OtherActor))->TakeDamage(10);
		Destroy();
	}
}
void ABullet::Throw(FVector Vel)
{
	ProjectileMovement->Velocity = Vel;
}


