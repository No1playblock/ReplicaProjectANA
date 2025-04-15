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
    FVector WidgetLocation = FVector(ActorLocation.X, ActorLocation.Y, ActorLocation.Z + 200); // �Ӹ��� 100
    //�̰� sprite size�� �����;� �ɰŰ����� ĳ���� �ؼ� ���������� �ᱹ interactCharcter �̷��ŷ� ��ӹ޾Ƽ� �ű⿡ spritecomponent �����;� �ҵ�.

    FVector2D ScreenPosition;

    this->SetRelativeLocation(WidgetLocation); // WidgetComponent�� ��� ��ġ�� �����մϴ�. �ʿ信 ���� ������ �� �ֽ��ϴ�.

    FVector PlayerCameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();

    FVector Direction = PlayerCameraLocation - GetOwner()->GetActorLocation();

    //Direction.Z = 0.f; // �յ� ����

    Direction.Y = 0.f;
    //�÷��̾� �������� �������� �̰� �ּ� Ǯ�� ��

    Direction.Normalize();

    // ���⺤�ͷ� ȸ���� ���
    FRotator NewRotation = FRotationMatrix::MakeFromX(Direction).Rotator();

    this->SetWorldRotation(NewRotation);
}