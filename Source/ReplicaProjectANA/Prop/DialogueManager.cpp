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

void ADialogueManager::SearchDialogue(FString NextID, FString ConvID)           //다음 대화의 위치를 잡아서 Result에 넣는다.
{

    for (FDialogueData* Row : AllRows)      //시작하는 곳의 Row를 찾아서 넣었음
    {
        if (Row->ConversationID == ConvID && Row->ID == NextID)         //현재 아이디가 받아온 저장했던 다음아이디 위치고, 대화 아이디도 같은거.
        {
            Result = nullptr;
            Result = Row;
            UE_LOG(LogTemp, Warning, TEXT("Find Row :  ConversationID: %s, ID: %s, Speaker: %s, Participants: %s, Text: %s"), *Result->ConversationID, *Result->ID, *Result->Speaker, *Result->Participants, *Result->Text);
        }
    }
    
}
void ADialogueManager::StartTalking(FString Speaker, FString ConvID)        //대화 시작세팅 ( 대화 시작부분 찾고, 대화참여자 배열에 액터 넣고)
{
    if (!BaseDialogueDT)
        return;

    for (FDialogueData* Row : AllRows)      //시작하는 곳의 Row를 찾아서 넣었음
    {
        if (Row->ConversationID == ConvID && Row->Speaker == Speaker && Row->ID == TEXT("Start"))
        {
            Result = Row;
            UE_LOG(LogTemp, Warning, TEXT("Find Row :  ConversationID: %s, ID: %s, Speaker: %s, Participants: %s, Text: %s"), *Result->ConversationID, *Result->ID, *Result->Speaker, *Result->Participants, *Result->Text);
        }
    }
    TArray<FString> ParticipantsNameArray;

    if (Result) //Result의 ParticiPants의 String값을 ,로 구분하여 Array에 넣음.
    {
        Result->Participants.ParseIntoArray(ParticipantsNameArray, TEXT(","), true);
    }

    for (FString Participant : ParticipantsNameArray)      //String Array에 있는 값을 태그로 찾아서 ActorArray에 넣음.
    {
        UE_LOG(LogTemp, Warning, TEXT("ParticipantString: %s"), *Participant);
        ParicipantActors.Add(FindTaggedActor(GetWorld(), FName(*Participant)));
    }

    for (AActor* SpeakActor : ParicipantActors)     //액터배열에서 말하는 애를 찾는다.
    {
        if (SpeakActor != nullptr && SpeakActor->Tags[0].ToString() == Result->Speaker)    //업데이트 한 Row의 말하는 애를 찾아서 말 시킴.     
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
        //있으면 
    }
    else
    {
        SaveNextID = Result->NextID;
        SaveConvID = Result->ConversationID;
    }
    
    
    //participant 가져와서 걔네의 Speaker Dialogue메소드 호출
    //시작할때 캐릭터멈추고, Participants widget보이게 하고
    //끝날때 캐릭터 움직이고, Participants widget 안 보이게 하고 참여자 배열 비우기
}
bool ADialogueManager::ProgressingDialogue()
{

    if (Result->ID == TEXT("End"))
    {
        DialogueEnd();
        UE_LOG(LogTemp, Warning, TEXT("End"));
        return false;
    }

    SearchDialogue(SaveNextID, SaveConvID);     //다음 대화의 Result(Row값)를 업데이트 //얘가 문제?
    if (Result->ChoiceText.Len() > 0)
    {
        TArray<FString> Choices, ChoiceIDs;
        Result->ChoiceText.ParseIntoArray(Choices, TEXT("|"), true);
        Result->NextID.ParseIntoArray(ChoiceIDs, TEXT("|"), true);

        // 선택지 UI 위젯 생성 및 Choice, ChoiceIDs 전달
     
        ShowChoiceWidget(Choices, ChoiceIDs);
        return true; // 선택지 선택 후 다음 대화는 유저가 선택할 때 진행
    }

    for (AActor* SpeakActor : ParicipantActors)     //액터배열에서 말하는 애를 찾는다.
    {
        if (SpeakActor != nullptr && SpeakActor->Tags[0].ToString() == Result->Speaker)    //업데이트 한 Row의 말하는 애를 찾아서 말 시킴.     
        {
            //SpeakActor도 가져와지고 Result도 가져와짐
            // 근데 메소드 호출이 안되네. 캐스팅 문제인가
            Cast<IDialogueInterface>(SpeakActor)->Dialogue(Result->Text);
            //UE_LOG(LogTemp, Warning, TEXT("SpeakActor Name: %s"), *SpeakActor->GetFName().ToString());
        }
    }

    
    SaveNextID = Result->NextID;
    return true;
}
void ADialogueManager::ShowChoiceWidget(const TArray<FString>& Choices, const TArray<FString>& ChoiceIDs)
{
    //위젯을 띄우고 버튼 누르면 OnChoiceSelected 호출
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    APawn* PlayerPawn = PC->GetPawn();
    if (!PlayerPawn) return;

    // 플레이어가 선택지 UI를 가지고 있는지 확인
    AANACharacter* Player = Cast<AANACharacter>(PlayerPawn); 
    if (!Player || !Player->GetTextBallonWidgetComponent()) return;

    // 해당 WidgetComponent에서 위젯 가져오기
    UUserWidget* WidgetInstance = Player->GetTextBallonWidgetComponent()->GetUserWidgetObject();
    UDialogueChoiceWidget* ChoiceWidget = Cast<UDialogueChoiceWidget>(WidgetInstance);
    if (!ChoiceWidget) return;

    UE_LOG(LogTemp, Warning, TEXT("InitChoices"));
    // 선택지 UI 초기화 및 바인딩
    ChoiceWidget->InitChoices(Choices, ChoiceIDs);
    ChoiceWidget->OnChoiceSelected.AddDynamic(this, &ADialogueManager::OnChoiceButtonSelected);

    // 위젯을 보이게
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


