// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABGASWeaponBox.h"
#include "AbilitySystemBlueprintLibrary.h"

void AABGASWeaponBox::NotifyActorBeginOverlap(AActor* Other)
{
	Super::NotifyActorBeginOverlap(Other);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Other, WeaponEventTag, FGameplayEventData());// Other가 플레이어니까 이쪽으로 이벤트가 감, 받기 위해서는 콜백 함수를 설정해야함
}
