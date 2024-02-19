// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABGASHpBarUserWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>
#include <Tag/ABGameplayTag.h>


void UABGASHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(UABCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UABCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnMaxHealthChanged);
		ASC->RegisterGameplayTagEvent(ABTAG_CHARACTER_INVINSIBLE, EGameplayTagEventType::NewOrRemoved ).AddUObject(this, &UABGASHpBarUserWidget::OnInvinsibleTagChange); // 보고있는 태그가 바뀔때 이벤트가 발생함
		PbHpBar->SetFillColorAndOpacity(HealthColor);

		const UABCharacterAttributeSet* CurrentCharacterAttributeSet = ASC->GetSet<UABCharacterAttributeSet>();
		if (CurrentCharacterAttributeSet)
		{
			CurrentHealth = CurrentCharacterAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentCharacterAttributeSet->GetMaxHealth();
			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHPBar();
			}
		}
	}
}

void UABGASHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHPBar();
}

void UABGASHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHPBar();
}

void UABGASHpBarUserWidget::OnInvinsibleTagChange(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		PbHpBar->SetFillColorAndOpacity(InvinvibleColor);
		PbHpBar->SetPercent(1.0f);
	}
	else
	{
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHPBar();
	}
	
}

void UABGASHpBarUserWidget::UpdateHPBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}

	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%0.f"), CurrentHealth, CurrentMaxHealth)));
	}
}
