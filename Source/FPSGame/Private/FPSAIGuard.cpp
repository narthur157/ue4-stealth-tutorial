#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "AIController.h"
#include "Net/UnrealNetwork.h"

AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentPatrolNodeIndex = 0;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

void AFPSAIGuard::HandleSeePawn(APawn* Pawn)
{
	if (Pawn == nullptr)
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->PauseMove(AIController->GetCurrentMoveRequestID());
	}

	SetGuardState(EAIState::Alerted);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(Pawn, false);
	}
	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);
}

void AFPSAIGuard::HandleHearNoise(APawn* Pawn, const FVector& Location, float Volume)
{
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("HandleHearNoise"))

	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->PauseMove(AIController->GetCurrentMoveRequestID());
		AIController->SetFocalPoint(Location);
	}

	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);

	SetGuardState(EAIState::Suspicious);
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted) {
		return;
	}

	SetGuardState(EAIState::Idle);
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->ResumeMove(AIController->GetCurrentMoveRequestID());
	}
	SetActorRotation(OriginalRotation);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::HandleSeePawn);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::HandleHearNoise);

	if (PatrolNodes.Num() > 1) {
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->ReceiveMoveCompleted.AddDynamic(this, &AFPSAIGuard::HandleMoveComplete);
			MoveToNextPatrolNode();
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("No AIController found on FPSAIGuard"));
		}
	}
}

void AFPSAIGuard::MoveToNextPatrolNode()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		if (bPatrollingFoward && CurrentPatrolNodeIndex == PatrolNodes.Num() - 1) 
		{
			bPatrollingFoward = false;
		}
		else if (!bPatrollingFoward && CurrentPatrolNodeIndex == 0) 
		{
			bPatrollingFoward = true;
		}

		CurrentPatrolNodeIndex = bPatrollingFoward ? CurrentPatrolNodeIndex + 1 : CurrentPatrolNodeIndex - 1;

		AIController->MoveToActor(PatrolNodes[CurrentPatrolNodeIndex]);
	}
}

void AFPSAIGuard::HandleMoveComplete(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("MOVE COMPLETE"));
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		MoveToNextPatrolNode();
	}
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnGuardStateChange(GuardState);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;
	OnRep_GuardState();
	OnGuardStateChange(NewState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}

