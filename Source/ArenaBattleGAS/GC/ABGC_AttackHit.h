// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "ABGC_AttackHit.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGC_AttackHit : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UABGC_AttackHit();

	/*
		�̰� ���µ� ���ڱ�..?
		Called when a GameplayCue is executed, this is used for instant effects or periodic ticks
		UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "GameplayCueNotify")
		bool OnExecute(AActor* MyTarget, const FGameplayCueParameters& Parameters) const;
	*/
	virtual bool OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=GameplayCue)
	TObjectPtr<class UParticleSystem> ParticleSystem;
};
