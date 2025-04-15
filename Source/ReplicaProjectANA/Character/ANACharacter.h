// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "Interface/DamagableCharacter.h"
#include "Character/BaseCharacter.h"
#include "Utility/InputBufferStruct.h"
#include "InputActionValue.h"
#include "ANACharacter.generated.h"

class UTextBallonWidgetComponent;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Indoor UMETA(DisplayName = "Indoor"),
	Outdoor UMETA(DisplayName = "Outdoor"),
};


class UInputMappingContext;
class UInputAction;

UCLASS()
class REPLICAPROJECTANA_API AANACharacter : public ABaseCharacter, public IDamagableCharacter
{
	GENERATED_BODY()
	
public: 
	AANACharacter();

	// AN_AnimationEnd���� ȣ���ϱ� ���� public���� ����, ��� �� ���� �ʿ�
	void AnimationEndNotifyEvent();

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(int32 DamageAmount) override;

	UFUNCTION(BlueprintCallable)
	ECharacterState GetCharacterInAndOutState();

	UFUNCTION(BlueprintCallable)
	void SetCharacterInAndOutState(ECharacterState State);

	FORCEINLINE UTextBallonWidgetComponent* GetTextBallonWidgetComponent() { return TextBallonWidgetComponent; }
	

protected:
	virtual void BeginPlay() override;
	
	

	void Move(const FInputActionValue& Value);

	// ĳ���� ���� ���� ����
	void SetInputBuffer(FInputBuffer& inputBuffer, bool isValid, FInputBufferType type, float launchSpeed, float launchDuration, float coolDownTime, FVector launchDirection);
	void InputDash();
	void InputAttack();
	void ExecuteDash(FInputBuffer inputBuffer);
	void ExecuteAttack(FInputBuffer inputBuffer);

	virtual void LaunchCoolDown() override;

	void AttackEnd();

	void ExecuteInputBuffer();

	UFUNCTION(BlueprintCallable)
	FVector GetCharacterDirection();

	UFUNCTION(BlueprintCallable)
	int GetCharacterRotation(FVector CharacterDirection);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

	DECLARE_DELEGATE_OneParam(TakeDamageDelegate, int32);

	void HitStopEnd();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	/** Attack Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> InteractAction;


	int32 MaxHealth;
	int32 CurrentHealth;

	// ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool bIsHitted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	bool bIsAttackAnimationPlaying;

	bool bIsAttackAnimationEnd;
	bool bIsLaunchCoolDownEnd;

	bool bIsDashing;

	// ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	FInputBuffer InputBuffer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	int CharacterRotation;


	// ��Ʈ ��ž ���� ����
	FTimerHandle HitStopTimerHandle;
	float HitStopDuration;


	// ������ ��, ī�޶� �Ÿ�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// ���� ī�޶� 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;



	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	//class UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTextBallonWidgetComponent> TextBallonWidgetComponent;


	class UCharacterMovementComponent* CharacterMovement;

	ECharacterState InAndOutDoorState = ECharacterState::Outdoor;
};

