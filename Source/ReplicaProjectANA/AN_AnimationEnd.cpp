// Fill out your copyright notice in the Description page of Project Settings.

#include "AN_AnimationEnd.h"
#include "Character/ANACharacter.h"
#include "PaperZDAnimInstance.h"

// ANACharacter의 pulblic 메서드를 직접 호출하므로, 주의해서 사용해야 함.
void UAN_AnimationEnd::OnReceiveNotify_Implementation(UPaperZDAnimInstance* OwningInstance) const
{
	AActor* OwningActor = OwningInstance ? OwningInstance->GetOwningActor() : nullptr;
	AANACharacter* Character = Cast<AANACharacter>(OwningActor);

	if (Character) {
		Character->AnimationEndNotifyEvent();
	}
}
