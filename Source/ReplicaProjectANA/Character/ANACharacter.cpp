// Fill out your copyright notice in the Description page of Project Settings.

#include "ANACharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/SaveLoadClass.h"
#include "MyGameInstance.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "UI/TextBallonWidgetComponent.h"

AANACharacter::AANACharacter()
	: bIsHitted(false),	bIsAttacking(false), bIsAttackAnimationPlaying(false), bIsAttackAnimationEnd(false), bIsLaunchCoolDownEnd(false),
	CharacterRotation(-1), HitStopDuration(.100f)
{
	UE_LOG(LogTemp, Warning, TEXT("ANACharacterConstructor"));
	//BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	//BoxCollision->InitBoxExtent(FVector(71.f, 45.f, 40.f));
	//BoxCollision->SetupAttachment(RootComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1500.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	TextBallonWidgetComponent = CreateDefaultSubobject<UTextBallonWidgetComponent>(TEXT("TextBallonWidgetComponent"));
	TextBallonWidgetComponent->SetWorldLocation(GetActorLocation());

	CharacterMovement = GetCharacterMovement();
	
	SetCharacterInAndOutState(ECharacterState::Outdoor);

	SetInputBuffer(InputBuffer, false, FInputBufferType::NONE, 0.f, 0.f, 0.f, FVector::ZeroVector);
}

void AANACharacter::BeginPlay()
{
	Super::BeginPlay();
	UMyGameInstance* MyGI= Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGI) 	{
		MyGI->Init();
		MyGI->Load();
	} else {
		UE_LOG(LogTemp, Warning, TEXT("NOT GI"));
	}
	
	if (CharacterMovement) {
		CharacterMovement->MaxWalkSpeed = 880.f;
		CharacterMovement->MaxAcceleration = 10000.f;
		CharacterMovement->GravityScale = 9.8f;
	}
}

ECharacterState AANACharacter::GetCharacterInAndOutState()
{
	return InAndOutDoorState;
}

void AANACharacter::SetCharacterInAndOutState(ECharacterState State)
{
	InAndOutDoorState = State;		//상태를 바꾸고
	TArray<AActor*> ActorswithTag;
	
	if (GetCharacterInAndOutState() == ECharacterState::Indoor)
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Outdoor"), ActorswithTag);
		for (AActor* actor : ActorswithTag)
		{
			actor->SetActorHiddenInGame(true);
		}
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Indoor"), ActorswithTag);
		for (AActor* actor : ActorswithTag)
		{
			actor->SetActorHiddenInGame(false);
		}
	}
	else
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Outdoor"), ActorswithTag);
		for (AActor* actor : ActorswithTag)
		{
			actor->SetActorHiddenInGame(false);
		}
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Indoor"), ActorswithTag);
		for (AActor* actor : ActorswithTag)
		{
			actor->SetActorHiddenInGame(true);
		}
	}
}

void AANACharacter::Move(const FInputActionValue& Value)
{
	if (GetWorldTimerManager().IsTimerActive(LaunchCoolDownTimerHandle)) {
		return;
	}

	if (bIsAttacking || bIsDashing) {
		return;
	}
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

FVector AANACharacter::GetCharacterDirection()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FVector MousePosition;

	PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

	FVector WorldLocation;
	FVector WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldLocation, WorldDirection);

	FVector Start = WorldLocation;
	FVector End = Start + WorldDirection * 10000;
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	// WorldStatic 오브젝트 타입만을 대상으로 하는 라인 트레이싱 쿼리
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.f);
	//DrawDebugPoint(GetWorld(), HitResult.Location, 5.f,FColor::Green, false, 5.f);

	if (GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams, CollisionParams)) {
		// z값은 배제
		FVector MouseDirection = HitResult.Location - GetActorLocation();
		MouseDirection.Z = 0.f;

		MouseDirection = MouseDirection.GetSafeNormal();

		// UE_LOG(LogTemp, Warning, TEXT("Meth : %f %f %f"), MouseDirection.X, MouseDirection.Y, MouseDirection.Z);
		return MouseDirection;
	}
	return FVector(0.f, 0.f, 0.f);
}

int AANACharacter::GetCharacterRotation(FVector CharacterDirection)
{
	FVector ForwardVector = CharacterDirection;  // 주어진 단위 벡터
	float X = ForwardVector.X;
	float Y = ForwardVector.Y;

	if ((FMath::Abs(X) > FMath::Abs(Y))) {
		// 0 1 2 3

		if (X > 0) {
			// UP
			return 0;
		} else {
			// DOWN
			return 1;
		}
	} 
	else {
		if (Y < 0) {
			// LEFT
			return 2;
		} else {
			// Right
			return 3;
		}
	}
	return -1;
}

void AANACharacter::SetInputBuffer(FInputBuffer& inputBuffer, bool isValid, FInputBufferType type, float launchSpeed, float launchDuration, float coolDownTime, FVector launchDirection)
{
	inputBuffer.IsValid = isValid;
	inputBuffer.Type = type;
	inputBuffer.LaunchSpeed = launchSpeed;
	inputBuffer.LaunchDuration = launchDuration;
	inputBuffer.CoolDownTime = coolDownTime;
	inputBuffer.LaunchDirection = launchDirection;
}

void AANACharacter::InputDash()
{
	if (bIsAttacking || bIsDashing) {
		if (!InputBuffer.IsValid) {
			SetInputBuffer(InputBuffer, true, FInputBufferType::DASH, 3000.f, 0.3f, 0.001f, GetCharacterDirection());
			return;
		}
	}
	else {
		FInputBuffer temp;
		SetInputBuffer(temp, false, FInputBufferType::DASH, 5000.f, 0.3f, 0.001f, GetCharacterDirection());
		ExecuteDash(temp);
	}
}

void AANACharacter::ExecuteDash(FInputBuffer inputBuffer)
{
	if (!bIsDashing) {
		LaunchSetup(inputBuffer.LaunchSpeed, inputBuffer.LaunchDuration, inputBuffer.LaunchDuration, inputBuffer.LaunchDirection);
	}
}

void AANACharacter::InputAttack()
{
	if (bIsAttacking || bIsDashing) {
		if (!InputBuffer.IsValid) {
			SetInputBuffer(InputBuffer, true, FInputBufferType::ATTACK, 10.f, 0.1f, 0.001f, GetCharacterDirection());
			// SetInputBuffer(InputBuffer, true, FInputBufferType::ATTACK, 1.f, 0.001f, 0.001f, GetCharacterDirection());
		}
	} 
	else {
		FInputBuffer temp;
		SetInputBuffer(temp, false, FInputBufferType::ATTACK, 10.f, 0.1f, 0.001f, GetCharacterDirection());
		// SetInputBuffer(InputBuffer, true, FInputBufferType::ATTACK, 1.f, 0.001f, 0.001f, GetCharacterDirection());
		ExecuteAttack(temp);
	}
}

void AANACharacter::ExecuteAttack(FInputBuffer inputBuffer)
{
	// 공격 방향 설정
	CharacterRotation = GetCharacterRotation(inputBuffer.LaunchDirection);

	if (!bIsAttacking) {
		bIsAttacking = true;
		bIsAttackAnimationPlaying = true;
		LaunchSetup(inputBuffer.LaunchSpeed, inputBuffer.LaunchDuration, inputBuffer.LaunchDuration, inputBuffer.LaunchDirection);
	}
}

void AANACharacter::AnimationEndNotifyEvent()
{
	// UE_LOG(LogTemp, Warning, TEXT("AnimationEndNotifyEvent"));
	if (bIsAttacking) {
		bIsAttackAnimationPlaying = false;
		bIsAttackAnimationEnd = true;
		AttackEnd();
	}
}

// Override, 블루프린트에 쿨타임 종료 이벤트를 보내기 위해 사용. 
void AANACharacter::LaunchCoolDown()
{
	// UE_LOG(LogTemp, Warning, TEXT("LaunchCoolDown"));
	if (bIsAttacking) {
		bIsLaunchCoolDownEnd = true;
		AttackEnd();
		return;
	}

	if (bIsDashing) {
		bIsDashing = false;
		ExecuteInputBuffer();
		return;
	}
}

void AANACharacter::AttackEnd()
{
	//UE_LOG(LogTemp, Warning, TEXT("AttackEnd"));
	//UE_LOG(LogTemp, Warning, TEXT("bIsAttackAnimationEnd : %d"), bIsAttackAnimationEnd ? 1 : 0);
	//UE_LOG(LogTemp, Warning, TEXT("bIsLaunchCoolDownEnd : %d"), bIsLaunchCoolDownEnd ? 1 : 0);
	if (bIsAttackAnimationEnd && bIsLaunchCoolDownEnd) {
		bIsAttacking = false;
		bIsAttackAnimationEnd = false;
		bIsLaunchCoolDownEnd = false;

		ExecuteInputBuffer();
	}
}

void AANACharacter::ExecuteInputBuffer()
{
	switch (InputBuffer.Type)
	{
		case FInputBufferType::NONE :
			UE_LOG(LogTemp, Warning, TEXT("FInputBufferType::NONE"));
			break;

		case FInputBufferType::ATTACK:
			UE_LOG(LogTemp, Warning, TEXT("FInputBufferType::ATTACK"));
			if (InputBuffer.IsValid && !bIsAttacking) {
				ExecuteAttack(InputBuffer);
				SetInputBuffer(InputBuffer, false, FInputBufferType::NONE, 0.f, 0.f, 0.f, FVector::ZeroVector);
			}
			break;

		case FInputBufferType::DASH:
			UE_LOG(LogTemp, Warning, TEXT("FInputBufferType::DASH"));
			if (InputBuffer.IsValid && !bIsDashing) {
				ExecuteDash(InputBuffer);
				SetInputBuffer(InputBuffer, false, FInputBufferType::NONE, 0.f, 0.f, 0.f, FVector::ZeroVector);
			}
			break;

		default:
			UE_LOG(LogTemp, Warning, TEXT("FInputBufferType::default"));
			break;
	}
	return;
}

void AANACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GetbIsLaunching()) {
		LaunchPerform();
	}

	//UE_LOG(LogTemp, Warning, TEXT("bIsAttacking : %d"), bIsAttacking ? 1 : 0);
	//UE_LOG(LogTemp, Warning, TEXT("bIsLaunchEnd : %d"), bIsLaunchCoolDownEnd ? 1 : 0);
	//UE_LOG(LogTemp, Warning, TEXT("bIsAttackAnimationStart : %d"), bIsAttackAnimationStart ? 1 : 0);
	//UE_LOG(LogTemp, Warning, TEXT("bISAttackAnimationEnd : %d"), bIsAttackAnimationEnd ? 1 : 0);
} 

void AANACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	PlayerInputComponent->BindAction<TakeDamageDelegate>(TEXT("TakeDamage"), IE_Pressed, this, &AANACharacter::TakeDamage, 30);

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AANACharacter::InputDash);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AANACharacter::Move);

		

		//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AANACharacter::InputAttack);

		// Moving
		//EnhancedInputComponent->BindAction<TakeDamageDelegate>(TEXT("TakeDamage"), ETriggerEvent::Triggered, this, &AANACharacter::TakeDamage, 30);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AANACharacter::InputAttack);
}

void AANACharacter::TakeDamage(int32 DamageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Take Damage! Amount : %d"), DamageAmount);

	bIsHitted = true;

	GetWorldTimerManager().SetTimer(HitStopTimerHandle, this, &AANACharacter::HitStopEnd, HitStopDuration, false, -1.0f);
	LaunchSetup(-1200.f, 0.05f, 0.001f, GetCharacterDirection());
	
	// 히트 스탑
	// CustomTimeDilation = 0.1f;
	GetWorldSettings()->SetTimeDilation(0.4f);

	// 체력 감소
	CurrentHealth -= DamageAmount;
}

void AANACharacter::HitStopEnd()
{
	// CustomTimeDilation = 1.0f;
	GetWorldSettings()->SetTimeDilation(1.f);
	bIsHitted = false;
	// 넉백
}