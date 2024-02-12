// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ABAT_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpandWaitForLandingDelegate);
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABAT_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()

public:
	UABAT_JumpAndWaitForLanding();

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="JumpAndWaitForLanding",
	HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UABAT_JumpAndWaitForLanding* CreateTask(UGameplayAbility* OwningAbility);

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UPROPERTY(BlueprintAssignable)
	FJumpandWaitForLandingDelegate OnComplete;

protected:
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLandedSignature, const FHitResult&, Hit);
	UFUNCTION()
	void OnLandedCallback(const FHitResult& Hit);
};
