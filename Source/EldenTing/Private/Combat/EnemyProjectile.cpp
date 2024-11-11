// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"

// Sets default values
AEnemyProjectile::AEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DestroyTimerHandle{};

	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &AEnemyProjectile::DestroyProjectile, 10.0f);
	
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyProjectile::HandleBeginOverlap(AActor* OtherActor)
{
	APawn* PawnRef = Cast<APawn>(OtherActor);

	if(!PawnRef->IsPlayerControlled()) { return; }

	FindComponentByClass<UParticleSystemComponent>()->SetTemplate(HitTemplate);
	FindComponentByClass<UProjectileMovementComponent>()->StopMovementImmediately();

	FTimerHandle DeathTimerHandle{};

	FindComponentByClass<USphereComponent>()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemyProjectile::DestroyProjectile, 1.3f);

	FDamageEvent ProjectileDamageEvent;
	
	PawnRef->TakeDamage(Damage, ProjectileDamageEvent, PawnRef->GetController(), this);
}

void AEnemyProjectile::DestroyProjectile()
{
	Destroy();
}

