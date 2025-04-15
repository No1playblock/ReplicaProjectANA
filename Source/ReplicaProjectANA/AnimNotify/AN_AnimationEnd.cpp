// Fill out your copyright notice in the Description page of Project Settings.

#include "AN_AnimationEnd.h"
#include "Character/ANACharacter.h"
#include "PaperZDAnimInstance.h"

// ANACharacter�� pulblic �޼��带 ���� ȣ���ϹǷ�, �����ؼ� ����ؾ� ��.
void UAN_AnimationEnd::OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	AActor* OwningActor = OwningInstance ? OwningInstance->GetOwningActor() : nullptr;
	AANACharacter* Character = Cast<AANACharacter>(OwningActor);

	if (Character) {
		Character->AnimationEndNotifyEvent();
	}
}
