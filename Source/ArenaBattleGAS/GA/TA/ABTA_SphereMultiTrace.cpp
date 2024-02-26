// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/TA/ABTA_SphereMultiTrace.h"
#include "ArenaBattleGAS.h"
#include <Abilities/GameplayAbility.h>
#include "GameframeWork/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Physics/ABCollision.h"
#include <Attribute/ABCharacterSkillAttributeSet.h>

FGameplayAbilityTargetDataHandle AABTA_SphereMultiTrace::MakeTargetData() const
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (ASC == nullptr)
	{
		return FGameplayAbilityTargetDataHandle();
	}
	const UABCharacterSkillAttributeSet* SkillAttributeSet = ASC->GetSet<UABCharacterSkillAttributeSet>();
	if (SkillAttributeSet == nullptr)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	TArray<FOverlapResult> Overlaps; // 범위에 있는 모든 객체들의 정보
	const float SkillRadius = SkillAttributeSet->GetSkillRange();
	FVector Origin = Character->GetActorLocation();
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(AABTA_SphereMultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_ABACTION, FCollisionShape::MakeSphere(SkillRadius), QueryParams);


	TArray<TWeakObjectPtr<AActor>> HitActors;
	for (const auto& Overlap : Overlaps)
	{
		AActor *HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && HitActors.Contains(HitActor) == false)
		{
			HitActors.Add(HitActor);
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bShowDebug)
	{
		FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}
#endif

	FGameplayAbilityTargetData_ActorArray* ActorData = new FGameplayAbilityTargetData_ActorArray();
	ActorData->SetActors(HitActors);

	return FGameplayAbilityTargetDataHandle(ActorData);
}
