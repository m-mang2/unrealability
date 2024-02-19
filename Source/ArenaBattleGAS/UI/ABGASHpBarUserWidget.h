// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ABGASUserWidget.h"
#include "GameplayEffectTypes.h"
#include "ABGASHpBarUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API UABGASHpBarUserWidget : public UABGASUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void SetAbilitySystemComponent(AActor* InOwner);

	virtual void OnHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	virtual void OnInvinsibleTagChange(const FGameplayTag CallbackTag, int32 NewCount);

	void UpdateHPBar();
protected:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UProgressBar> PbHpBar;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> TxtHpStat;

	float CurrentHealth = 0.0f;
	float CurrentMaxHealth = 0.1;

	FLinearColor HealthColor = FLinearColor::Red;
	FLinearColor InvinvibleColor = FLinearColor::Blue;
};
