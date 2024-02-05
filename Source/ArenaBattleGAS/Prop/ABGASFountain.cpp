// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABGASFountain.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemComponent.h"
//#include "GA/ABGA_Rotate.h"
#include "Tag/ABGameplayTag.h"
#include "Abilities/GameplayAbility.h"

AABGASFountain::AABGASFountain()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	ActionPeriod = 3.0f;
}

UAbilitySystemComponent* AABGASFountain::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RotatingMovement->bAutoActivate = false; // 자동 활성화 제거
	RotatingMovement->Deactivate(); // 비활성화

	ASC->InitAbilityActorInfo( // 시작할때 반드시 사용해야함
	this, // nOwnerActor는 실제 asc를 소유하고 구동
	this); //데이터를 처리하진 않지만 비주얼만 처리

	//FGameplayAbilitySpec RotateSkillSpec(UABGA_Rotate::StaticClass());
	//ASC->GiveAbility(RotateSkillSpec);
	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
}

void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(
		ActionTimer,
		this, // 호출 함수의 객체
		&AABGASFountain::TimerAction, // 호출될 함수
		ActionPeriod, // 주기 
		true, // 반복
		0.0f); // 바로시작
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);

	if (ASC->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING) == false) // 어빌리티가 발동중이 아니라면
	{
		ASC->TryActivateAbilitiesByTag(TargetTag);
	}
	else
	{
		ASC->CancelAbilities(&TargetTag);
	}


	/*FGameplayAbilitySpec* RotateGASpec = ASC->FindAbilitySpecFromClass(UABGA_Rotate::StaticClass());
	if (RotateGASpec == nullptr)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("no rotate spec found"));

		return;
	}

	if (RotateGASpec->IsActive())
	{
		ASC->TryActivateAbility(RotateGASpec->Handle);
	}
	else
	{
		ASC->CancelAbilityHandle(RotateGASpec->Handle);
	}*/

	/*if (RotatingMovement->IsActive() == false)
	{
		RotatingMovement->Activate(true);
	}
	else
	{
		RotatingMovement->Deactivate();
	}*/
}
