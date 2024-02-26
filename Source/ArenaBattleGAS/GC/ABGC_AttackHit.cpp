// Fill out your copyright notice in the Description page of Project Settings.


#include "GC/ABGC_AttackHit.h"
#include <Particles/ParticleSystem.h>
#include "Kismet/GameplayStatics.h" //? ����

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
	//FGameplayCueParameters�� ���������� ����
	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult(); // �浹����� ������ ������ �־������, �ȳ����� ����
	if (HitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation( Target,ParticleSystem, HitResult->ImpactPoint, FRotator::ZeroRotator, true);
	}
	else // �ټ��� ������ ���� �� ���⼭ ó��
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
