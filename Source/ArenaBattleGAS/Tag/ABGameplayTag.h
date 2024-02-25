
#pragma once

#include "GameplayTagContainer.h"

#define ABTAG_ACTOR_ROTATE FGameplayTag::RequestGameplayTag(FName("Actor.Action.Rotate"))
#define ABTAG_ACTOR_ISROTATING FGameplayTag::RequestGameplayTag(FName("Actor.State.IsRotating")) // 태그가 실행되면 이 부분이 설정된다는데 어떻게..?
#define ABTAG_DATA_DAMAGE FGameplayTag::RequestGameplayTag(FName("Data.Damage"))
#define ABTAG_CHARACTER_ISDEAD FGameplayTag::RequestGameplayTag(FName("Character.State.IsDead"))
#define ABTAG_CHARACTER_INVINSIBLE FGameplayTag::RequestGameplayTag(FName("Character.State.Invinsible"))
#define ABTAG_GAMEPLAYCUE_CHARACTER_ATTACKHIT FGameplayTag::RequestGameplayTag(FName("GameplayCue.Character.AttackHit"))

#define ABTAG_EVENT_CHARACTER_WEAPONEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Wepone.Equip"))
#define ABTAG_EVENT_CHARACTER_WEAPONUNEQUIP FGameplayTag::RequestGameplayTag(FName("Event.Character.Wepone.Unequip"))