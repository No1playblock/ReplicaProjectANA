// Fill out your copyright notice in the Description page of Project Settings.


#include "SlicedObject.h"
#include "Kismet/GameplayStatics.h"
#include "PaperSprite.h"
#include "Engine/Texture2D.h"
#include "GameFramework/Character.h"
// Sets default values
ASlicedObject::ASlicedObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    

    TopSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TopSprite"));
    TopSprite->SetupAttachment(BoxComponent);

    BottomSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BottomSprite"));
    BottomSprite->SetupAttachment(BoxComponent);
  
    bIsSliced = false;
}

// Called when the game starts or when spawned
void ASlicedObject::BeginPlay()
{
	Super::BeginPlay();
    PickRandomObject();
	
}
void ASlicedObject::SetComponent()
{
    
    TopSprite->SetSprite(TopSpriteArray[RandomIndex]);
    BottomSprite->SetSprite(BottomSpriteArray[RandomIndex]);

   // FVector2D BottomSpriteSize = BottomSprite->GetSprite()->GetSourceSize();
    UTexture2D* BottomTexture = BottomSprite->GetSprite()->GetBakedTexture();
    UTexture2D* TopTexture = TopSprite->GetSprite()->GetBakedTexture();
    
    FVector2D BottomSpriteSize = FVector2D(BottomTexture->GetSizeX(), BottomTexture->GetSizeY());
    FVector2D TopSpriteSize = FVector2D(TopTexture->GetSizeX(), TopTexture->GetSizeY());
    
    //FVector2D TopSpriteSize = TopSprite->GetSprite()->GetSourceSize();
    
    BoxComponent->SetBoxExtent(FVector(10.0f, BottomSpriteSize.X, (BottomSpriteSize.Y + TopSpriteSize.Y) / 2));
    //BoxComponent->InitBoxExtent(FVector(10.0f, BottomSpriteSize.X, (BottomSpriteSize.Y + TopSpriteSize.Y) / 2));
    BoxComponent->SetRelativeLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z+(BottomSpriteSize.Y + TopSpriteSize.Y) / 2.0f));      //z축의 위치를 배치한 높이에 + 해야됨
    
    BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    BoxComponent->SetGenerateOverlapEvents(true);
  
    TopSprite->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
    BottomSprite->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

    //UE_LOG(LogTemp, Warning, TEXT("%s"), *SpriteSize.ToString());
    
    BottomSprite->SetRelativeLocation(FVector(0.0f, 0.0f, -TopSpriteSize.Y));
    //
    ////rotation setup
    TopSprite->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    BottomSprite->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

   
}
void ASlicedObject::PickRandomObject()
{
    int32 ArrayLength = TopSpriteArray.Num();

    if (ArrayLength > 0)
    {
        // Get a random index between 0 and ArrayLength - 1
        RandomIndex = FMath::RandRange(0, ArrayLength - 1);
        SetComponent();
    }
    
    
}

void ASlicedObject::Sliced()
{
    UE_LOG(LogTemp, Warning, TEXT("Sliced"));
    if (!bIsSliced) {
        TopSprite->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        TopSprite->SetSimulatePhysics(true);
        FVector playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
        FVector objectLocation = GetActorLocation();

        FVector forceVector = (objectLocation - playerLocation).GetSafeNormal2D()*150000.0f;
        
        TopSprite->AddImpulse(forceVector);
    }
    
    bIsSliced = true;
}


void ASlicedObject::NotifyActorBeginOverlap(AActor* OtherActor)
{
	
    //Sliced();
    UE_LOG(LogTemp, Warning, TEXT("Overlap"));
}


