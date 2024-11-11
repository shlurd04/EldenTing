// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraceComponent.generated.h"

class UArrowComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ELDENTING_API UTraceComponent : public UActorComponent
{
	GENERATED_BODY()

	UStaticMeshComponent* Sword;

	UArrowComponent* SwordBottom;
	
	UArrowComponent* SwordTop;

	UPROPERTY(EditAnywhere)
	double BoxCollisionLength = 10.0;

	UPROPERTY(EditAnywhere)
	bool bDebugMode = false;

	TArray<AActor*> TargetsToIgnore;

public:	
	// Sets default values for this component's properties
	UTraceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

	UPROPERTY(VisibleAnywhere)
	bool bIsAttacking = false;
};
