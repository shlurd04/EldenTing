// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/Fighter.h"
#include "Engine/DamageEvents.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	Sword = Cast<UStaticMeshComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("sword")));
	
	SwordBottom = Cast<UArrowComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("Sword Bottom")));
	SwordTop = Cast<UArrowComponent>(GetOwner()->GetDefaultSubobjectByName(TEXT("Sword Top")));

	if(!Sword || !SwordBottom || !SwordTop)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sword or Sword bottom or top missing"));
	}
	
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!bIsAttacking) {return;}

	FVector Start = SwordBottom->GetComponentLocation();
	FVector End = SwordTop->GetComponentLocation();
	FQuat SwordRotation = Sword->GetComponentTransform().GetRotation();

	double WeaponDistance = FVector::Dist(Start, End);

	FVector BoxHalfExtent { BoxCollisionLength, BoxCollisionLength, WeaponDistance };
	BoxHalfExtent /= 2;

	FCollisionShape Box = FCollisionShape::MakeBox(BoxHalfExtent);
	
	TArray<FHitResult> OutResults;

	FCollisionQueryParams IgnoreParams{
		FName{TEXT("Ignore Params")},
		false,
		GetOwner()
	};
	
	bool bHasFoundTargets = GetWorld()->SweepMultiByChannel(OutResults, Start, End, FQuat::Identity, ECC_GameTraceChannel1, Box, IgnoreParams);

	if(bDebugMode)
	{
		FVector CentrePoint = UKismetMathLibrary::VLerp(
			Start, End, 0.5f
		);
		
		UKismetSystemLibrary::DrawDebugBox(
			GetWorld(),
			CentrePoint,
			Box.GetExtent(),
			bHasFoundTargets ? FLinearColor::Red : FLinearColor::Green,
			SwordRotation.Rotator(),
			1,
			2
		);
	}

	if(OutResults.Num() == 0) { return; }

	float CharacterDamage = 0.0f;

	IFighter* FighterRef = Cast<IFighter>(GetOwner());

	if(FighterRef)
	{
		CharacterDamage += FighterRef->GetDamage();
	}

	FDamageEvent TargetAttackedEvent;

	for(const FHitResult& Hit : OutResults)
	{
		AActor* TargetActor = Hit.GetActor();

		if(TargetsToIgnore.Contains(TargetActor)) { continue; }
		
		TargetActor->TakeDamage(CharacterDamage, TargetAttackedEvent, GetOwner()->GetInstigatorController(), GetOwner());

		TargetsToIgnore.AddUnique(TargetActor);
	}
}

void UTraceComponent::HandleResetAttack()
{
	TargetsToIgnore.Empty();
}

