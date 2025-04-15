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
#include "Math/UnrealMathUtility.h"  // FMath ���

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
	Tags.Add(FName(*Name));						//NPC�� �±׸� Name���� ����(DialogueManager���� �±׷� �����ϱ� ����)

	if (FoundActors.Num()>0)
	{
		
		if (FoundActors[0])
		{
			DialogueMgr = Cast<ADialogueManager>(FoundActors[0]);
		}
	}
	
	
}
void ANPC::Interact()	//E ����� DialogueManager�� �� �̸� ������
{
	UUserWidget* PressEWidget = InteractComponent->GetWidget();

	DialogueWidget = Cast<UInteractPromptWidget>(TextBallonWidgetComponent->GetWidget());
	
	// ������ ���̺��� ��(Row) �˻�
	
	if (!DialogueWidget || !PressEWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("DialogueWidget is null!"));
		return;
	}
	PressEWidget->SetRenderOpacity(0.0f);
	DialogueWidget->SetRenderOpacity(1.0f);	//�̰� �ٸ� NPC�鵵 �� �Ѿ��ҵ�
	

	if (bTalking)	//��ȭ���̸� ��ȣ�ۿ��� DialogueManager�� �ѱ��. �ƴ϶�� ������
	{
		bTalking = DialogueMgr->ProgressingDialogue();			//��ȭ�� ������ false��ȯ
		UE_LOG(LogTemp, Warning, TEXT("ProgressingDialogue"));
	}
	else
	{
		DialogueMgr->StartTalking(Name, ConversationID);
		bTalking = true;
	}

}

void ANPC::Dialogue(FString SubtitleString)	//������ �ִ� WidgetComponent�� ��ȭ ������ ���� ����, ��ȭ������ DialogueManager�� ���� �޴´�.
{
	DialogueWidget->SetRenderOpacity(1.0f);
	DialogueWidget->SetSubTitleText(SubtitleString);
}
void ANPC::DialogueWidgetInvisible()
{
	DialogueWidget->SetRenderOpacity(0.0f);
}
