// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Enemy.h"
#include "Characters/EEnemyState.h"
#include "Interfaces/Fighter.h"
#include "BossCharacter.generated.h"

class UCombatComponent;
class UBlackboardComponent;
class UStatsComponent;

UCLASS()
class ELDENTING_API ABossCharacter : public ACharacter, public IEnemy, public IFighter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemyState> InitialState;

	UBlackboardComponent* BlackboardComp;

public:
	// Sets default values for this character's properties
	ABossCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStatsComponent* StatsComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCombatComponent* CombatComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect);

	virtual float GetDamage() override;

	virtual void Attack() override;

	virtual float GetAnimDuration() override;

};