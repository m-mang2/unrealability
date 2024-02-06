// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Attack.h"
#include "Character/ABCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameFramework/CharacterMovementComponent.h"

UABGA_Attack::UABGA_Attack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UABGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//  ĳ���Ϳ��� ��Ÿ�� ���
	AABCharacterBase* ABCharacter = Cast<AABCharacterBase>(ActorInfo->AvatarActor.Get());
	if (ABCharacter)
	{
		ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), ABCharacter->GetComboActionMontage());

		// ���� �ݹ��� �����ؾ���
		PlayAttackTask->OnCompleted.AddDynamic(this, &UABGA_Attack::OnCompleteCallback); // DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMontageWaitSimpleDelegate); �Ű����� ���� �ݹ�
		PlayAttackTask->OnInterrupted.AddDynamic(this, &UABGA_Attack::OnInterruptedCallback);

		PlayAttackTask->ReadyForActivation(); // �̰� ȣ���ؾ� �����̵�
	}
}

void UABGA_Attack::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UABGA_Attack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	AABCharacterBase* ABCharacter = Cast<AABCharacterBase>(ActorInfo->AvatarActor.Get());
	if (ABCharacter)
	{
		ABCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void UABGA_Attack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
}

void UABGA_Attack::OnCompleteCallback()
{	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UABGA_Attack::OnInterruptedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
