// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"
#include "AbilitySystemComponent.h"
#include "Player/ABGASPlayerState.h"
#include "EnhancedInputComponent.h"
#include "GameplayAbilitySpec.h"
#include "UI/ABGASWidgetComponent.h"
#include "UI/ABGASUserWidget.h"
#include <Attribute/ABCharacterAttributeSet.h>
#include <Tag/ABGameplayTag.h>



AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	// ASC를 생성하면 2개가 되어 관리가 어려움
	// 그래서 빙의되면 그때 PlayerState에 있는 값을 가져옴
	ASC = nullptr;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattleGAS/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Succeeded())
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	HpBar =CreateDefaultSubobject<UABGASWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidget(TEXT("/Game/ArenaBattle/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidget.Succeeded())
	{
		HpBar->SetWidgetClass(HpBarWidget.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_Hellhammer/SK_Blunt_HellHammer.SK_Blunt_HellHammer'"));
	if (WeaponMeshRef.Succeeded())
	{
		WeaponMesh = WeaponMeshRef.Object;
	}

	WeaponRange = 75.0;
	WeaponAttackRate = 100.0;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SkillActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattleGAS/Animation/AM_SkillAttack.AM_SkillAttack'"));
	if (SkillActionMontageRef.Succeeded())
	{
		SkillActionMontage = SkillActionMontageRef.Object;
	}
}

UAbilitySystemComponent* AABGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

// 빙의할때 불려지는 함수로 네트워크 프로그래밍에서는 서버에서만 호출
void AABGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AABGASPlayerState* GASPS =  GetPlayerState<AABGASPlayerState>();
	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		ASC->InitAbilityActorInfo(GASPS, this);

		ASC->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &AABGASCharacterPlayer::EquipWeapon);
		ASC->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &AABGASCharacterPlayer::UnequipWeapon);

		const UABCharacterAttributeSet* AttributeSet = ASC->GetSet<UABCharacterAttributeSet>();
		if (AttributeSet)
		{
			AttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::OnOutOfHealth);
		}

		int32 InputID = 0;
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			//StartSpec.InputID = InputID++;
			ASC->GiveAbility(StartSpec);
		}
		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			ASC->GiveAbility(StartSpec);
		}
		

		SetupGASInputComponent();

		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		if (PlayerController)
		{
			PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
		}
	}

}

void AABGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetupGASInputComponent();
}

void AABGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AABGASCharacterPlayer::GASInputReleased, 0);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 1);

		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 2);
	}
}

void AABGASCharacterPlayer::GASInputPressed(int32 InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AABGASCharacterPlayer::GASInputReleased(int32 InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void AABGASCharacterPlayer::OnOutOfHealth()
{
	SetDead();
}

void AABGASCharacterPlayer::EquipWeapon(const FGameplayEventData* EventData)
{
	if (Weapon && WeaponMesh)
	{
		Weapon->SetSkeletalMesh(WeaponMesh);

		FGameplayAbilitySpec NewSkillSpec(SkillAbilityClass);
		NewSkillSpec.InputID = 2; // 입력의 구별이 필요
		if (ASC->FindAbilitySpecFromClass(SkillAbilityClass) == false)
		{
			ASC->GiveAbility(NewSkillSpec);
		}


		// 바람직한 방법은 아니나...이런것도 있다.. 이펙트 사용할것
		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute());
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute());

		ASC->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange + WeaponRange);
		ASC->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate + WeaponAttackRate);
	}
}

void AABGASCharacterPlayer::UnequipWeapon(const FGameplayEventData* EventData)
{
	if (Weapon)
	{
		Weapon->SetSkeletalMesh(nullptr);

		FGameplayAbilitySpec* FindSkillSpec = ASC->FindAbilitySpecFromClass(SkillAbilityClass);
		if (FindSkillSpec)
		{
			ASC->ClearAbility(FindSkillSpec->Handle);
		}

		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute());
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute());

		ASC->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange - WeaponRange);
		ASC->SetNumericAttributeBase(UABCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate - WeaponAttackRate);
	}
}
