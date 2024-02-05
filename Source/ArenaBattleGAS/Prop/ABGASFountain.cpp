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

	RotatingMovement->bAutoActivate = false; // �ڵ� Ȱ��ȭ ����
	RotatingMovement->Deactivate(); // ��Ȱ��ȭ

	ASC->InitAbilityActorInfo( // �����Ҷ� �ݵ�� ����ؾ���
	this, // nOwnerActor�� ���� asc�� �����ϰ� ����
	this); //�����͸� ó������ ������ ���־� ó��

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
		this, // ȣ�� �Լ��� ��ü
		&AABGASFountain::TimerAction, // ȣ��� �Լ�
		ActionPeriod, // �ֱ� 
		true, // �ݺ�
		0.0f); // �ٷν���
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);

	if (ASC->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING) == false) // �����Ƽ�� �ߵ����� �ƴ϶��
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
