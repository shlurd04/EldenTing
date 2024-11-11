// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_ChargeAttack.h"
#include "AIController.h"
#include "Animations/BossAnimInstance.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ControllerRef = OwnerComp.GetAIOwner();
	CharacterRef = ControllerRef->GetCharacter();
	BossAnim = Cast<UBossAnimInstance>(CharacterRef->GetMesh()->GetAnimInstance());

	BossAnim->bIsCharging = true;

	UE_LOG(LogTemp, Warning, TEXT("Charging"));

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("isReadyToCharge"), false);

	bIsFinished = false;
	
	return EBTNodeResult::InProgress;
}

UBTT_ChargeAttack::UBTT_ChargeAttack()
{
	bNotifyTick = true;

	MoveCompletedDelegate.BindUFunction(this, "HandleMoveCompleted");
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();
	FVector PlayerLocation = PlayerRef->GetActorLocation();

	FAIMoveRequest MoveRequest {PlayerLocation};
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(AcceptableRadius);

	ControllerRef->MoveTo(MoveRequest);
	ControllerRef->SetFocus(PlayerRef);

	ControllerRef->ReceiveMoveCompleted.AddUnique(MoveCompletedDelegate);

	OriginalWalkSpeed = CharacterRef->GetCharacterMovement()->MaxWalkSpeed;

	CharacterRef->GetCharacterMovement()->MaxWalkSpeed = ChargeWalkSpeed;
}

void UBTT_ChargeAttack::HandleMoveCompleted()
{
	BossAnim->bIsCharging = false;

	FTimerHandle AttackTimerHandle;

	CharacterRef->GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &UBTT_ChargeAttack::FinishAttackTask, 1.0f, false);

	CharacterRef->GetCharacterMovement()->MaxWalkSpeed = OriginalWalkSpeed;
}

void UBTT_ChargeAttack::FinishAttackTask()
{
	bIsFinished = true;
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bIsReadyToCharge = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("isReadyToCharge"));

	if(bIsReadyToCharge)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("isReadyToCharge"), false);

		ChargeAtPlayer();
	}

	if(!bIsFinished) { return; }

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);

	ControllerRef->ReceiveMoveCompleted.Remove(MoveCompletedDelegate);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
