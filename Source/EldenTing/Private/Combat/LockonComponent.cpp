#include "Combat/LockonComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/Enemy.h"

ULockonComponent::ULockonComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();
	PlayerController = GetWorld()->GetFirstPlayerController();
	MovementComp = OwnerRef->GetCharacterMovement();
	SpringArmComp = OwnerRef->FindComponentByClass<USpringArmComponent>();
	
}

void ULockonComponent::StartLockon(float Radius)
{
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	FHitResult Hit;

	FCollisionQueryParams params;
	
	bool bHasFoundTarget = GetWorld()->SweepSingleByChannel(Hit, CurrentLocation, CurrentLocation, FQuat::Identity, ECC_GameTraceChannel1, Sphere, params);

	if(bHasFoundTarget)
	{
		if(!Hit.GetActor()->Implements<UEnemy>()) {return;}
		
		CurrentTargetActor = Hit.GetActor();
		
		PlayerController->SetIgnoreLookInput(true);
		MovementComp->bOrientRotationToMovement = false;
		MovementComp->bUseControllerDesiredRotation = true;
		MovementComp->MaxWalkSpeed = 450;

		IEnemy::Execute_OnSelect(CurrentTargetActor);
		OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
	}
}

void ULockonComponent::EndLockon()
{
	IEnemy::Execute_OnDeselect(CurrentTargetActor);
	
	CurrentTargetActor = nullptr;
	PlayerController->ResetIgnoreLookInput();
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = false;
	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
	MovementComp->MaxWalkSpeed = 200;
}

void ULockonComponent::ToggleLockon(float Radius)
{
	if(IsValid(CurrentTargetActor))
	{
		EndLockon();
	}
	else
	{
		StartLockon(Radius);
	}
}

void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(IsValid(CurrentTargetActor))
	{
		FVector CurrentLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = CurrentTargetActor->GetActorLocation();

		double TargetDistance = FVector::Distance(CurrentLocation, TargetLocation);

		if(TargetDistance >= BreakDistance)
		{
			EndLockon();
			return;
		}

		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

		TargetLocation.Z -= 125;
		
		PlayerController->SetControlRotation(NewRotation);
	}
	
}

