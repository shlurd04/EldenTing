// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Characters/StatsComponent.h"
#include "Animations/PlayerAnimInstance.h"
#include "Characters/EStat.h"
#include "Characters/PlayerActionsComponent.h"
#include "Combat/BlockComponent.h"
#include "Combat/CombatComponent.h"
#include "Combat/LockonComponent.h"
#include "Combat/TraceComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("Stats Component"));
	LockonComp = CreateDefaultSubobject<ULockonComponent>(TEXT("Lockon Component"));
	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("Trace Component"));
	BlockComp = CreateDefaultSubobject<UBlockComponent>(TEXT("Block Component"));
	PlayerActionsComp = CreateDefaultSubobject<UPlayerActionsComponent>(TEXT("Player Actions Component"));

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMainCharacter::GetDamage()
{
	return StatsComp->Stats[EStat::Strength];
}

bool AMainCharacter::HasEnoughStamina(float Cost)
{
	return StatsComp->Stats[EStat::Stamina] >= Cost;
}

