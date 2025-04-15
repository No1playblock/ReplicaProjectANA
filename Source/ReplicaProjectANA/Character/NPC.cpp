// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "UI/InteractionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameData/DialogueDataBase.h"
#include "UI/InteractPromptWidget.h"
#include "UI/TextBallonWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DialogueManager.h"
#include "Math/UnrealMathUtility.h"  // FMath 사용

ANPC::ANPC()
{
	
	FRotator MyRotation(0.0f, 90.0f, -60.0f);

	GetSprite()->SetRelativeLocation(FVector(36.96f, 0.0f, 4.019238f));
	GetSprite()->SetRelativeRotation(MyRotation.Quaternion());
	GetSprite()->SetRelativeScale3D(FVector(0.5f));

	GetCapsuleComponent()->InitCapsuleSize(120.0f, 120.0f);

	InteractComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractComponent"));
	TextBallonWidgetComponent = CreateDefaultSubobject<UTextBallonWidgetComponent>(TEXT("TextBallonWidgetComponent"));
	TextBallonWidgetComponent->SetWorldLocation(GetActorLocation());

	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> CharacterMeshRef(TEXT("/Script/Paper2D.PaperFlipbook'/Game/Assets/Animation/Flipbooks/DummyMob/Attack/T_DummyMonsterAttackDown.T_DummyMonsterAttackDown'"));
	if (CharacterMeshRef.Object)
	{
		GetSprite()->SetFlipbook(CharacterMeshRef.Object);
	}

	//DialogueWidget = Cast<UInteractPromptWidget>(TextBallonWidgetComponent->GetWidgetClass());
	
	
	if (!DialogueWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("DialogueWidget is null!"));
	}	

}
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	DialogueWidget = Cast<UInteractPromptWidget>(TextBallonWidgetComponent->GetWidget());
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADialogueManager::StaticClass(), FoundActors);
	Tags.Add(FName(*Name));						//NPC의 태그를 Name으로 변경(DialogueManager에서 태그로 접근하기 위함)

	if (FoundActors.Num()>0)
	{
		
		if (FoundActors[0])
		{
			DialogueMgr = Cast<ADialogueManager>(FoundActors[0]);
		}
	}
	
	
}
void ANPC::Interact()	//E 지우고 DialogueManager로 내 이름 보내기
{
	UUserWidget* PressEWidget = InteractComponent->GetWidget();

	DialogueWidget = Cast<UInteractPromptWidget>(TextBallonWidgetComponent->GetWidget());
	
	// 데이터 테이블에서 행(Row) 검색
	
	if (!DialogueWidget || !PressEWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("DialogueWidget is null!"));
		return;
	}
	PressEWidget->SetRenderOpacity(0.0f);
	DialogueWidget->SetRenderOpacity(1.0f);	//이건 다른 NPC들도 다 켜야할듯
	

	if (bTalking)	//대화중이면 상호작용을 DialogueManager에 넘긴다. 아니라면 얘한테
	{
		bTalking = DialogueMgr->ProgressingDialogue();			//대화가 끝나면 false반환
		UE_LOG(LogTemp, Warning, TEXT("ProgressingDialogue"));
	}
	else
	{
		DialogueMgr->StartTalking(Name, ConversationID);
		bTalking = true;
	}

}

void ANPC::Dialogue(FString SubtitleString)	//가지고 있는 WidgetComponent에 대화 내용을 띄우는 역할, 대화내용은 DialogueManager로 부터 받는다.
{
	DialogueWidget->SetRenderOpacity(1.0f);
	DialogueWidget->SetSubTitleText(SubtitleString);
}
void ANPC::DialogueWidgetInvisible()
{
	DialogueWidget->SetRenderOpacity(0.0f);
}
