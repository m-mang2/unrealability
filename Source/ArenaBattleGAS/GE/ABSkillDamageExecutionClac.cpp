// Fill out your copyright notice in the Description page of Project Settings.


#include "GE/ABSkillDamageExecutionClac.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterSkillAttributeSet.h"
#include "Attribute/ABCharacterAttributeSet.h"

void UABSkillDamageExecutionClac::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	if (SourceASC == nullptr || TargetASC == nullptr)
	{
		return;
	}

	AActor* SourceActor = SourceASC->GetAvatarActor();
	AActor* TargetActor = TargetASC->GetAvatarActor();

	if (SourceActor == nullptr || TargetActor == nullptr)
	{
		return;
	}

	const float MaxDamageRange = SourceASC->GetNumericAttributeBase(UABCharacterSkillAttributeSet::GetMaxSkillRangeAttribute());
	const float MaxDamage = SourceASC->GetNumericAttributeBase(UABCharacterSkillAttributeSet::GetMaxSkillAttackRateAttribute());
	
	const float Distance = FMath::Clamp(SourceActor->GetDistanceTo(TargetActor), 0.0f, MaxDamageRange);
	const float InvDamageRatio = 1.0f - Distance /MaxDamageRange;

	float Damage = InvDamageRatio * MaxDamage;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UABCharacterAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
}
