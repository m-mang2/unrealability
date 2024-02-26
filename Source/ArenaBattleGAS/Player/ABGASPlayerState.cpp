// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABGASPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include <Attribute/ABCharacterSkillAttributeSet.h>

AABGASPlayerState::AABGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	//ASC->SetIsReplicated(true); // 네트워크를 고려한다면 Replicate설정을 해야줘야함

	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>(TEXT("AttributeSet"));
	SkillAttributeSet = CreateDefaultSubobject<UABCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));
}

UAbilitySystemComponent* AABGASPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
