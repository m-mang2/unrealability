// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_AttackHitCheck.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemblueprintLibrary.h"
#include "GA/AT/ABAT_Trace.h"
#include "GA/TA/ABTA_Trace.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include <Tag/ABGameplayTag.h>

UABGA_AttackHitCheck::UABGA_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ABGAS_LOG(LogABGAS, Log, TEXT("Begin %s"), *TriggerEventData->EventTag.GetTagName().ToString());

	CurrentLevel = TriggerEventData->EventMagnitude;

	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	UABAT_Trace* AttackTraceTask = UABAT_Trace::CreateTask(this, TargetActorClass); // 판정을 여기서함
	if (AttackTraceTask)
	{
		AttackTraceTask->OnComplete.AddDynamic(this, &UABGA_AttackHitCheck::OnTraceResultCallBack);
		AttackTraceTask->ReadyForActivation();
	}
}

void UABGA_AttackHitCheck::OnTraceResultCallBack(const FGameplayAbilityTargetDataHandle& TargetDatahandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDatahandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDatahandle, 0);
		
		ABGAS_LOG(LogABGAS, Log, TEXT("Target %s Detected") , *HitResult.GetActor()->GetName());

		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (TargetASC == nullptr || SourceASC == nullptr)
		{
			ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found"));
			return;
		}
		/*
		UABCharacterAttributeSet* TargetAttribute = const_cast<UABCharacterAttributeSet*>(TargetASC->GetSet<UABCharacterAttributeSet>());
		if (SourceAttribute == nullptr || TargetAttribute == nullptr)
		{
			ABGAS_LOG(LogABGAS, Error, TEXT("ASC not found"));
			return;
		}

		const float AttackDamage = SourceAttribute->GetAttackRate();
		TargetAttribute->SetHealth(TargetAttribute->GetHealth() - AttackDamage);


		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		const UABCharacterAttributeSet* SourceAttribute = SourceASC->GetSet<UABCharacterAttributeSet>();
		*/

		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			//EffectSpecHandle.Data->SetSetByCallerMagnitude(ABTAG_DATA_DAMAGE, -SourceAttribute->GetAttackRate());
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDatahandle);
	
			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddHitResult(HitResult);

			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			TargetASC->ExecuteGameplayCue(ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}

		FGameplayEffectSpecHandle BuffEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackBuffEffect); // AttackDamageEffect 잘못 넣으면 내가 맞음..
		if (BuffEffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, BuffEffectSpecHandle);
		}
	}
	else if(UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDatahandle, 0)) // Actor로 넘기는데 이걸로 확인 FGameplayAbilityTargetDataHandle AABTA_SphereMultiTrace::MakeTargetData()
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo_Checked();
		FGameplayEffectSpecHandle EffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentLevel);
		if (EffectSpecHandle.IsValid())
		{
			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, EffectSpecHandle, TargetDatahandle);

			FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(EffectSpecHandle);
			CueContextHandle.AddActors(TargetDatahandle.Data[0].Get()->GetActors());

			FGameplayCueParameters CueParam;
			CueParam.EffectContext = CueContextHandle;

			SourceASC->ExecuteGameplayCue(ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT, CueParam);
		}
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
