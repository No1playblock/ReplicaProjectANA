// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Decorator/BTDecCloseEnough.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecCloseEnough::UBTDecCloseEnough()
{
    NodeName = TEXT("CloseEnough");

    TargetToFollowKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTDecCloseEnough, TargetToFollowKey), AActor::StaticClass());
}

bool UBTDecCloseEnough::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();
    APawn* AIPawn = AIController ? AIController->GetPawn() : nullptr;

    if (!AIController || !AIPawn)
    {
        return false;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return false;
    }

    UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetToFollowKey.SelectedKeyName);
    AActor* TargetActor = Cast<AActor>(TargetObject);

    if (!TargetActor)
    {
        return false;
    }

    const FVector MyLocation = AIPawn->GetActorLocation();
    const FVector TargetLocation = TargetActor->GetActorLocation();

    const float Distance = FVector::Dist(MyLocation, TargetLocation);

    return Distance >= AcceptableDistance;
}
