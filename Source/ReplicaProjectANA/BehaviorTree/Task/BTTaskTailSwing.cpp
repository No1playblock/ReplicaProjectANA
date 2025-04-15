// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTaskTailSwing.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MobCharacter.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/ANACharacter.h"
UBTTaskTailSwing::UBTTaskTailSwing()
{
	NodeName = TEXT("TailSwing");
    IsTaskingKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskTailSwing, IsTaskingKey));
}

EBTNodeResult::Type UBTTaskTailSwing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* ControlledPawn= AIController ? AIController->GetPawn() : nullptr;

    if (!AIController || !ControlledPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    
    AMobCharacter* Mob = Cast<AMobCharacter>(ControlledPawn);

    if (!Mob || Mob->GetbIsAttacking()) return EBTNodeResult::Failed; // 이미 실행 중이면 무시

    Mob->SetbIsAttacking(true);
    UWorld* World = ControlledPawn->GetWorld();
    if (!World) return EBTNodeResult::Failed;

    
    FTimerHandle DelayTimerHandle;
    World->GetTimerManager().SetTimer(DelayTimerHandle, FTimerDelegate::CreateUObject(this, &UBTTaskTailSwing::ForloopWithDelay, &OwnerComp), 0.6f, false);

	return EBTNodeResult::InProgress;
}

void UBTTaskTailSwing::ForloopWithDelay(UBehaviorTreeComponent* OwnerComp)
{
    AAIController* AIController = OwnerComp->GetAIOwner();
    APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

    if (!AIController || !ControlledPawn)  return;

    Center = ControlledPawn->GetActorLocation();
    
    for (int32 i = 0; i < NumPoints; i++)
    {
        float AngleRad = FMath::DegreesToRadians(i * (360 / NumPoints));

        float Radius = TailRadius - CircleRadius;

        float X = FMath::Cos(AngleRad) * Radius + Center.X;
        float Y = FMath::Sin(AngleRad) * Radius + Center.Y;
        float Z = 150.0f;

        AttackPoints.Add(FVector(X, Y, Z));
    }

    //시계방향꼬리치기
    for (int32 i = 0; i < NumPoints; i++)
    {
        FTimerHandle SwingHandle;
        float DelayTime = i * (TailSwingTime / float(NumPoints-1));     //N번하면 간격은 N-1개임

        GetWorld()->GetTimerManager().SetTimer(SwingHandle, FTimerDelegate::CreateLambda([this, i]()
            {
                TailSwing(i);
            }), 
            DelayTime, false);
    }

    //반시계방향 꼬리치기
    for (int32 i = NumPoints-1; i >=0; i--)
    {
        FTimerHandle ReverseHandle;
        float DelayTime = TailSwingTime + ReverseTailDelay+ (NumPoints-1-i) * (ReverseTailSwingTime / float(NumPoints-1));


        GetWorld()->GetTimerManager().SetTimer(ReverseHandle, FTimerDelegate::CreateLambda([this, i]()
            {
                TailSwing(i);
            }),
            DelayTime, false);
    }
    FTimerHandle FinishTimerHandle;

    GetWorld()->GetTimerManager().SetTimer(FinishTimerHandle, FTimerDelegate::CreateUObject(this, &UBTTaskTailSwing::FinishTask, OwnerComp), TailSwingTime + ReverseTailSwingTime + 0.93f, false);
}

void UBTTaskTailSwing::TailSwing(int32 Index)
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_EngineTraceChannel1));

    TSubclassOf<AActor> FilterClass = AANACharacter::StaticClass();
    
    TArray<AActor*> OutActors;

    UE_LOG(LogTemp, Warning, TEXT("index: %d"), Index);
    bool bHit = UKismetSystemLibrary::SphereOverlapActors(GetWorld(),AttackPoints[Index], CircleRadius, ObjectTypes, FilterClass, {}, OutActors);
    DrawDebugCircle(
        GetWorld(),
        AttackPoints[Index],
        CircleRadius,
        20,                       // Segments
        FColor::Blue,            // Line color
        false,                   // bPersistentLines
        0.5f,                    // LifeTime
        0,                       // Depth Priority
        5.0f,                    // Thickness
        FVector(1, 0, 0),        // Y Axis
        FVector(0, 1, 0),        // Z Axis
        false                    // Draw Axis
    );
    if (bHit)
    {
        for (AActor* Actor : OutActors)
        {
            Cast<AANACharacter>(Actor)->TakeDamage(Damage);
            DrawDebugCircle(
                GetWorld(),
                AttackPoints[Index],
                CircleRadius,
                20,                       // Segments
                FColor::Red,            // Line color
                false,                   // bPersistentLines
                0.5f,                    // LifeTime
                0,                       // Depth Priority
                5.0f,                    // Thickness
                FVector(1, 0, 0),        // Y Axis
                FVector(0, 1, 0),        // Z Axis
                false                    // Draw Axis
            );
        }
    }
}

void UBTTaskTailSwing::FinishTask(UBehaviorTreeComponent* OwnerComp)
{

    if (!OwnerComp) return;

    APawn* ControlledPawn = OwnerComp->GetAIOwner()->GetPawn();
    if (AMobCharacter* Mob = Cast<AMobCharacter>(ControlledPawn))
    {
        Mob->SetbIsAttacking(false);
    }

    UBlackboardComponent* BlackboardComp = OwnerComp->GetBlackboardComponent();
    if (BlackboardComp && IsTaskingKey.SelectedKeyName.IsValid())
    {
        BlackboardComp->SetValueAsBool(IsTaskingKey.SelectedKeyName, false);
    }
    AttackPoints.Reset();
    FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
