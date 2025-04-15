// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/Task/BTTaskMoveToPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NavigationSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UBTTaskMoveToPlayer::UBTTaskMoveToPlayer()
{
    NodeName = TEXT("MoveToPlayer");

    TargetToFollowKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTaskMoveToPlayer, TargetToFollowKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTaskMoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp) return EBTNodeResult::Failed;

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetToFollowKey.SelectedKeyName));
    if (!TargetActor) return EBTNodeResult::Failed;

    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(TargetActor);
    MoveRequest.SetAcceptanceRadius(AcceptanceRadius);

    FNavPathSharedPtr NavPath;
    FPathFollowingRequestResult MoveResult = AIController->MoveTo(MoveRequest, &NavPath);

    if (MoveResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
    {
        return EBTNodeResult::Succeeded;
    }

    if (MoveResult.Code == EPathFollowingRequestResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

    MoveRequestID = MoveResult.MoveId;
    CachedOwnerComp = &OwnerComp;

    // 이동 완료 이벤트 바인딩
    AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(
        this, &UBTTaskMoveToPlayer::OnMoveCompleted
    );

    return EBTNodeResult::InProgress;
    
}

void UBTTaskMoveToPlayer::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    if (RequestID != MoveRequestID || !CachedOwnerComp) return; 

    AAIController* AIController = CachedOwnerComp->GetAIOwner();
    if (AIController)
    {
        AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
    }

    EBTNodeResult::Type FinalResult = Result.IsSuccess()
        ? EBTNodeResult::Succeeded
        : EBTNodeResult::Failed;

    FinishLatentTask(*CachedOwnerComp, FinalResult);
}
