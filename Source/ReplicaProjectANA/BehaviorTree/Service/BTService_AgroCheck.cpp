// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AgroCheck.h"
#include "Controller/Mob_AIController.h"
#include "Character/MobCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "DrawDebugHelpers.h"




void UBTService_AgroCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBehaviorTree* ThisTree=nullptr;
	AMob_AIController* ThisController = nullptr;
	AMobCharacter* ThisMobCharacter = nullptr;
	
	if (ThisTree == nullptr || ThisController || nullptr || ThisMobCharacter == nullptr)
	{
		ThisTree = OwnerComp.GetCurrentTree();
		ThisController = Cast<AMob_AIController>(OwnerComp.GetAIOwner());
		ThisMobCharacter = Cast<AMobCharacter>(ThisController->GetPawn());

		if (ThisTree == nullptr || ThisController == nullptr || ThisMobCharacter == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Warning Agro Service performed on invalid AI"));
			return;
		}
	}

	ThisController->GetBlackboardComp()->SetValueAsVector(TEXT("HomeLocation"), ThisMobCharacter->GetActorLocation());

	
	FCollisionQueryParams SphereSweepParams(FName(TEXT("AgroCheckSweep")), true, ThisMobCharacter);

	FCollisionObjectQueryParams objectQuery(ECC_GameTraceChannel1);

	FHitResult HitOut(ForceInit);

	//DrawDebugSphere(ThisMobCharacter->GetWorld(), ThisMobCharacter->GetActorLocation(), Radius, 12, FColor::Red, false, 1.0f);

	bool bResult = ThisMobCharacter->GetWorld()->SweepSingleByObjectType(HitOut, ThisMobCharacter->GetActorLocation(),
																			ThisMobCharacter->GetActorLocation() + FVector(0.f, 0.f, -1000.0f),
																			FQuat(), objectQuery, FCollisionShape::MakeSphere(Radius), SphereSweepParams);


	if (bResult)
	{
		ThisController->GetBlackboardComp()->SetValueAsObject(TEXT("TargetToFollow"), HitOut.GetActor());
		ThisController->GetBlackboardComp()->SetValueAsVector(TEXT("TargetLocation"), HitOut.GetActor()->GetActorLocation());
		ThisController->GetBlackboardComp()->SetValueAsFloat(TEXT("AcceptDistance"), 150.0f);
		ThisController->GetBlackboardComp()->SetValueAsFloat(TEXT("Distance"), FVector::Dist2D(ThisMobCharacter->GetActorLocation(), HitOut.GetActor()->GetActorLocation()));
	}
	else
	{
		ThisController->GetBlackboardComp()->SetValueAsObject(TEXT("TargetToFollow"), nullptr);
	}
}
