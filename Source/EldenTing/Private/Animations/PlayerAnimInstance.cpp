#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::HandleUpdatedTarget(AActor* NewTargetActorRef)
{
	bIsInCombat = IsValid(NewTargetActorRef);
}

void UPlayerAnimInstance::UpdateDirection()
{
	APawn* PawnRef = TryGetPawnOwner();

	if (!IsValid(PawnRef)) {return;}

	if(!bIsInCombat) {return;}

	CurrentDirection = CalculateDirection(PawnRef->GetVelocity(), PawnRef->GetActorRotation());
	
}
