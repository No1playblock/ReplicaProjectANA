// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManager.h"
#include "GameData/DialogueDataBase.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/DialogueInterface.h"
#include "Character/NPC.h"
#include "Character/ANACharacter.h"
#include "UI/TextBallonWidgetComponent.h"
#include "UI/DialogueChoiceWidget.h"

// Called when the game starts or when spawned
void ADialogueManager::BeginPlay()
{
	Super::BeginPlay();

    static const FString ContextString(TEXT("Dialogue Context"));
    BaseDialogueDT->GetAllRows<FDialogueData>(ContextString, AllRows);
}

void ADialogueManager::SearchDialogue(FString NextID, FString ConvID)           //���� ��ȭ�� ��ġ�� ��Ƽ� Result�� �ִ´�.
{

    for (FDialogueData* Row : AllRows)      //�����ϴ� ���� Row�� ã�Ƽ� �־���
    {
        if (Row->ConversationID == ConvID && Row->ID == NextID)         //���� ���̵� �޾ƿ� �����ߴ� �������̵� ��ġ��, ��ȭ ���̵� ������.
        {
            Result = nullptr;
            Result = Row;
            UE_LOG(LogTemp, Warning, TEXT("Find Row :  ConversationID: %s, ID: %s, Speaker: %s, Participants: %s, Text: %s"), *Result->ConversationID, *Result->ID, *Result->Speaker, *Result->Participants, *Result->Text);
        }
    }
    
}
void ADialogueManager::StartTalking(FString Speaker, FString ConvID)        //��ȭ ���ۼ��� ( ��ȭ ���ۺκ� ã��, ��ȭ������ �迭�� ���� �ְ�)
{
    if (!BaseDialogueDT)
        return;

    for (FDialogueData* Row : AllRows)      //�����ϴ� ���� Row�� ã�Ƽ� �־���
    {
        if (Row->ConversationID == ConvID && Row->Speaker == Speaker && Row->ID == TEXT("Start"))
        {
            Result = Row;
            UE_LOG(LogTemp, Warning, TEXT("Find Row :  ConversationID: %s, ID: %s, Speaker: %s, Participants: %s, Text: %s"), *Result->ConversationID, *Result->ID, *Result->Speaker, *Result->Participants, *Result->Text);
        }
    }
    TArray<FString> ParticipantsNameArray;

    if (Result) //Result�� ParticiPants�� String���� ,�� �����Ͽ� Array�� ����.
    {
        Result->Participants.ParseIntoArray(ParticipantsNameArray, TEXT(","), true);
    }

    for (FString Participant : ParticipantsNameArray)      //String Array�� �ִ� ���� �±׷� ã�Ƽ� ActorArray�� ����.
    {
        UE_LOG(LogTemp, Warning, TEXT("ParticipantString: %s"), *Participant);
        ParicipantActors.Add(FindTaggedActor(GetWorld(), FName(*Participant)));
    }

    for (AActor* SpeakActor : ParicipantActors)     //���͹迭���� ���ϴ� �ָ� ã�´�.
    {
        if (SpeakActor != nullptr && SpeakActor->Tags[0].ToString() == Result->Speaker)    //������Ʈ �� Row�� ���ϴ� �ָ� ã�Ƽ� �� ��Ŵ.     
        {
            Cast<IDialogueInterface>(SpeakActor)->Dialogue(Result->Text);
        }
    }


    if (Result->ChoiceText.Len() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("ChoiceCHocie"));
        TArray<FString> Choices, ChoiceIDs;
        Result->ChoiceText.ParseIntoArray(Choices, TEXT("|"), true);
        Result->NextID.ParseIntoArray(ChoiceIDs, TEXT("|"), true);
        //������ 
    }
    else
    {
        SaveNextID = Result->NextID;
        SaveConvID = Result->ConversationID;
    }
    
    
    //participant �����ͼ� �³��� Speaker Dialogue�޼ҵ� ȣ��
    //�����Ҷ� ĳ���͸��߰�, Participants widget���̰� �ϰ�
    //������ ĳ���� �����̰�, Participants widget �� ���̰� �ϰ� ������ �迭 ����
}
bool ADialogueManager::ProgressingDialogue()
{

    if (Result->ID == TEXT("End"))
    {
        DialogueEnd();
        UE_LOG(LogTemp, Warning, TEXT("End"));
        return false;
    }

    SearchDialogue(SaveNextID, SaveConvID);     //���� ��ȭ�� Result(Row��)�� ������Ʈ //�갡 ����?
    if (Result->ChoiceText.Len() > 0)
    {
        TArray<FString> Choices, ChoiceIDs;
        Result->ChoiceText.ParseIntoArray(Choices, TEXT("|"), true);
        Result->NextID.ParseIntoArray(ChoiceIDs, TEXT("|"), true);

        // ������ UI ���� ���� �� Choice, ChoiceIDs ����
     
        ShowChoiceWidget(Choices, ChoiceIDs);
        return true; // ������ ���� �� ���� ��ȭ�� ������ ������ �� ����
    }

    for (AActor* SpeakActor : ParicipantActors)     //���͹迭���� ���ϴ� �ָ� ã�´�.
    {
        if (SpeakActor != nullptr && SpeakActor->Tags[0].ToString() == Result->Speaker)    //������Ʈ �� Row�� ���ϴ� �ָ� ã�Ƽ� �� ��Ŵ.     
        {
            //SpeakActor�� ���������� Result�� ��������
            // �ٵ� �޼ҵ� ȣ���� �ȵǳ�. ĳ���� �����ΰ�
            Cast<IDialogueInterface>(SpeakActor)->Dialogue(Result->Text);
            //UE_LOG(LogTemp, Warning, TEXT("SpeakActor Name: %s"), *SpeakActor->GetFName().ToString());
        }
    }

    
    SaveNextID = Result->NextID;
    return true;
}
void ADialogueManager::ShowChoiceWidget(const TArray<FString>& Choices, const TArray<FString>& ChoiceIDs)
{
    //������ ���� ��ư ������ OnChoiceSelected ȣ��
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    APawn* PlayerPawn = PC->GetPawn();
    if (!PlayerPawn) return;

    // �÷��̾ ������ UI�� ������ �ִ��� Ȯ��
    AANACharacter* Player = Cast<AANACharacter>(PlayerPawn); 
    if (!Player || !Player->GetTextBallonWidgetComponent()) return;

    // �ش� WidgetComponent���� ���� ��������
    UUserWidget* WidgetInstance = Player->GetTextBallonWidgetComponent()->GetUserWidgetObject();
    UDialogueChoiceWidget* ChoiceWidget = Cast<UDialogueChoiceWidget>(WidgetInstance);
    if (!ChoiceWidget) return;

    UE_LOG(LogTemp, Warning, TEXT("InitChoices"));
    // ������ UI �ʱ�ȭ �� ���ε�
    ChoiceWidget->InitChoices(Choices, ChoiceIDs);
    ChoiceWidget->OnChoiceSelected.AddDynamic(this, &ADialogueManager::OnChoiceButtonSelected);

    // ������ ���̰�
    Player->GetTextBallonWidgetComponent()->SetVisibility(true);
}
void ADialogueManager::OnChoiceButtonSelected(const FString& SelectedID)
{
    UE_LOG(LogTemp, Warning, TEXT("BindingSuccess"));
    SearchDialogue(SelectedID, SaveConvID);

    for (AActor* SpeakActor : ParicipantActors)
    {
        if (SpeakActor && SpeakActor->Tags[0].ToString() == Result->Speaker)
        {
            Cast<IDialogueInterface>(SpeakActor)->Dialogue(Result->Text);
        }
    }

    SaveNextID = Result->NextID;

    if (AANACharacter* Player = Cast<AANACharacter>(UGameplayStatics::GetPlayerPawn(this, 0)))
    {
        if (Player->GetTextBallonWidgetComponent())
        {
            Player->GetTextBallonWidgetComponent()->SetVisibility(false);
        }
    }
}
void ADialogueManager::DialogueEnd()
{
    SaveNextID = "";
    SaveConvID = "";
   

    for (AActor* SpeakActor : ParicipantActors)    
    {
        Cast<ANPC>(SpeakActor)->DialogueWidgetInvisible();
    }
    ParicipantActors.Reset();
}
AActor* ADialogueManager::FindTaggedActor(UWorld* World, FName Tag)
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(World, Tag, FoundActors);
    if (FoundActors.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("FindActor: %s"), *FoundActors[0]->GetFName().ToString());
    }
    else
        UE_LOG(LogTemp, Warning, TEXT("FindTaggedActor Num < 0"));
    return FoundActors.Num() > 0 ? FoundActors[0] : nullptr;
}


