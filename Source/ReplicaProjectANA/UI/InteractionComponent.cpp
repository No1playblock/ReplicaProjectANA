// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ANACharacter.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "PaperSprite.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    DetectionRadius = 900.0f;

    static ConstructorHelpers::FClassFinder<UUserWidget> InteractWidgetRef(TEXT("/Game/Blueprints/Widget/InteractionWidget.InteractionWidget_C"));

    if (InteractWidgetRef.Class)
    {
        //InteractionWidgetClass = InteractWidgetRef.Class;
        SetWidgetClass (InteractWidgetRef.Class);
    }
    SetCollisionEnabled(ECollisionEnabled::NoCollision);

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> InteractContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Interact.IMC_Interact'"));

    if (nullptr!=InteractContextRef.Object)
    {
        InteractContext = InteractContextRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> InteractActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Interact.IA_Interact'"));

    if (nullptr != InteractActionRef.Object)
    {
        InteractAction = InteractActionRef.Object;
    }
}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerPawn = PlayerController->GetPawn();

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &UInteractionComponent::PressE);
            UE_LOG(LogTemp, Warning, TEXT("Binding"));
        }

    }

    UE_LOG(LogTemp, Warning, TEXT("TickCOmponent %s"), *GetOwner()->GetFName().ToString());
}
void UInteractionComponent::PressE()
{
    if (DistanceToPlayer <= DetectionRadius)
    {
        UE_LOG(LogTemp, Warning, TEXT("Interact!"));
        IInteractionInterface* myInterface = Cast<IInteractionInterface>(GetOwner());
        if (myInterface)
        {
            myInterface->Interact();
        }
            
    }
}

// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
   
    if (!PlayerPawn) {

        return;
    }
    DistanceToPlayer = FVector::Dist(PlayerPawn->GetActorLocation(), GetOwner()->GetActorLocation());

    DistanceToPlayer <= DetectionRadius ? ShowInteractionPrompt(true) : ShowInteractionPrompt(false);
    
}
void UInteractionComponent::SetWidgetTransform()
{
    FVector ActorLocation = GetOwner()->GetActorLocation();
    //UGameplayStatics::GetPlayerCharacter()
    FVector WidgetLocation = FVector(ActorLocation.X, ActorLocation.Y, ActorLocation.Z + 200); // 머리위 100
    //이거 sprite size를 가져와야 될거같은데 캐스팅 해서 가져오려면 결국 interactCharcter 이런거로 상속받아서 거기에 spritecomponent 가져와야 할듯.

    FVector2D ScreenPosition;

    this->SetRelativeLocation(WidgetLocation); // WidgetComponent의 상대 위치를 설정합니다. 필요에 따라 변경할 수 있습니다.

    FVector PlayerCameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

    FVector Direction = PlayerCameraLocation - GetOwner()->GetActorLocation();

    //Direction.Z = 0.f; // 앞뒤 기울기

    Direction.Y = 0.f;
    //플레이어 방향으로 돌릴꺼면 이거 주석 풀면 됨

    Direction.Normalize();

    // 방향벡터로 회전값 계산
    FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

    this->SetWorldRotation(NewRotation);
}
void UInteractionComponent::ShowInteractionPrompt(bool bShow)
{
    
    if (bShow)
    {
        if (!(this->IsVisible()))
        {
            this->SetVisibility(true);
            check(this->IsVisible()); 
            UE_LOG(LogTemp, Warning, TEXT("visible"));

            if (PlayerController)		
            {
                if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
                {
                    Subsystem->AddMappingContext(InteractContext, 1);
                }
                
            }
        }
        SetWidgetTransform();
       
    }
    else
    {
        if (this->IsVisible())
        {

            this->SetVisibility(false);
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
            {
                Subsystem->RemoveMappingContext(InteractContext);
            }
            
        }
    }
}

