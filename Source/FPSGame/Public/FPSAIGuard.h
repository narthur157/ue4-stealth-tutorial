// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;
class AAIController;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(EditInstanceOnly, Category = "Patrol")
	TArray<AActor*> PatrolNodes;

	int32 CurrentPatrolNodeIndex;
	bool bPatrollingFoward;

	UFUNCTION()
	void MoveToNextPatrolNode();

	UFUNCTION()
	void HandleMoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	UFUNCTION()
	void HandleSeePawn(APawn* Pawn);

	UFUNCTION()
	void HandleHearNoise(APawn* Pawn, const FVector& Location, float Volume);

	FRotator OriginalRotation;

	UFUNCTION()
	void ResetOrientation();
	
	FTimerHandle TimerHandle_ResetOrientation;

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState = EAIState::Idle;

	UFUNCTION()
	void OnRep_GuardState();

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnGuardStateChange(EAIState NewSate);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	
};
