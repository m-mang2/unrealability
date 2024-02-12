// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/AT/ABAT_Trace.h"
#include <Abilities/GameplayAbilityTargetActor.h>
#include "AbilitySystemComponent.h"
UABAT_Trace::UABAT_Trace()
{
}

UABAT_Trace* UABAT_Trace::CreateTask(UGameplayAbility* OwningAbility, TSubclassOf<class AABTA_Trace> TargetActorClass)
{
	UABAT_Trace* Trace = NewAbilityTask<UABAT_Trace>(OwningAbility);
	Trace->TargetActorClass = TargetActorClass;
	return Trace;
}

void UABAT_Trace::Activate()
{
	Super::Activate();

	SpawndAndInitializeTargetActor();
	FinalizeTargetActor();

	SetWaitingOnAvatar();
}

void UABAT_Trace::OnDestroy(bool bInOwnerFinished)
{
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->Destroy();
	}
	Super::OnDestroy(bInOwnerFinished);
}

void UABAT_Trace::SpawndAndInitializeTargetActor()
{
	SpawnedTargetActor = Cast<AABTA_Trace>(Ability->GetWorld()->SpawnActorDeferred<AGameplayAbilityTargetActor>(TargetActorClass, FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	if (SpawnedTargetActor)
	{
		SpawnedTargetActor->SetShowDebug(true);
		SpawnedTargetActor->TargetDataReadyDelegate.AddUObject(this, &UABAT_Trace::OnTargetDataReadyCallback); // DECLARE_MULTICAST_DELEGATE_OneParam(FAbilityTargetData, const FGameplayAbilityTargetDataHandle&);
	}
}

void UABAT_Trace::FinalizeTargetActor()
{
	UAbilitySystemComponent*ASC = AbilitySystemComponent.Get();
	if (ASC)
	{
		const FTransform SpawndTrasform = ASC->GetAvatarActor()->GetTransform();
		SpawnedTargetActor->FinishSpawning(SpawndTrasform);
		ASC->SpawnedTargetActors.Push(SpawnedTargetActor);
		SpawnedTargetActor->StartTargeting(Ability);
		SpawnedTargetActor->ConfirmTargeting(); // 바로 확인하고 AABTA_Trace에 있는 ConfirmTargetingAndContinue를 실행
	}
}

void UABAT_Trace::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& DataHandle)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnComplete.Broadcast(DataHandle);
	}
}
