// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TextBallonWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UTextBallonWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (PlayerController)
    {
        SetWidgetTransform();
    }
   
}

void UTextBallonWidgetComponent::BeginPlay()
{
    Super::BeginPlay();
    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void UTextBallonWidgetComponent::SetWidgetTransform()
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