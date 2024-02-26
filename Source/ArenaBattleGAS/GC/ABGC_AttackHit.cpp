// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/ABGC_AttackHit.h"
#include <Particles/ParticleSystem.h>
#include "Kismet/GameplayStatics.h" //? 뭔대

UABGC_AttackHit::UABGC_AttackHit()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionRef(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (ExplosionRef.Succeeded())
	{
		ParticleSystem = ExplosionRef.Object;
	}
}

bool UABGC_AttackHit::OnExecute_Implementation(AActor* Target, const FGameplayCueParameters& Parameters) const
{
	//FGameplayCueParameters에 많은정보가 있음
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult(); // 충돌결과를 보내고 싶으면 넣어줘야함, 안넣으면 없음
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation( Target,ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	else // 다수의 정보가 들어올 때 여기서 처리
	{
		for (const auto& TargetActor : Parameters.EffectContext.Get()->GetActors() )
		{
			if (TargetActor.Get())
			{
				UGameplayStatics::SpawnEmitterAtLocation(Target, ParticleSystem, TargetActor.Get()->GetActorLocation(), FRotator::ZeroRotator, true);
			}
		}
	}

	return false;
}
