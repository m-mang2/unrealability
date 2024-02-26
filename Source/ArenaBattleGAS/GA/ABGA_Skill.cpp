// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Skill.h"
#include "Character/ABGASCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UABGA_Skill::UABGA_Skill()
{
}

void UABGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AABGASCharacterPlayer* TargetCharacter = Cast<AABGASCharacterPlayer>(ActorInfo->AvatarActor.Get());

	if(TargetCharacter == nullptr)
		return;


	ActiveSkillActionMontage = TargetCharacter->GetSkillActionMontage();
	if(ActiveSkillActionMontage == nullptr)
		return;
	TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	UAbilityTask_PlayMontageAndWait *PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), ActiveSkillActionMontage, 1.0f);
	if (PlayMontageTask)
	{
		PlayMontageTask->OnCompleted.AddDynamic(this, &UABGA_Skill::OnCompleteCallback);
		PlayMontageTask->OnInterrupted.AddDynamic(this, &UABGA_Skill::OnInterruptedCallback);

		PlayMontageTask->ReadyForActivation(); // Áß¿ä
	}
}

void UABGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AABGASCharacterPlayer* TargetCharacter = Cast<AABGASCharacterPlayer>(ActorInfo->AvatarActor.Get());

	if (TargetCharacter != nullptr)
	{
		TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UABGA_Skill::OnCompleteCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UABGA_Skill::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

