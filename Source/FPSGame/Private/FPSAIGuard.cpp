// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSAIGuardController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Engine/TargetPoint.h"
#include "FPSGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	AIControllerClass = AFPSAIGuardController::StaticClass();

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	MoveToNextTargetPoint();
	OriginalRotation = GetActorRotation();
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
		return;

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Yellow, false, 10.f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false, this);
	}

	SetGuardState(EAIState::Alerted);
}

void AFPSAIGuard::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
		return;

	DrawDebugSphere(GetWorld(), Location, 32.f, 12, FColor::Green, false, 10.f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandler_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandler_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.f);

	SetGuardState(EAIState::Suspicious);
}


void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
		return;

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
		return;
	
	GuardState = NewState;
	OnRep_GuardState();

	switch (GuardState)
	{
		case EAIState::Idle:
			MoveToNextTargetPoint();
			break;
		
		default:
			GetController()->StopMovement();
			break;
	}
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChange(GuardState);
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}

void AFPSAIGuard::MoveToNextTargetPoint()
{
	AFPSAIGuardController* controller = Cast<AFPSAIGuardController>(GetController());
	UE_LOG(LogTemp, Log, TEXT("MOVING TRY"));
	if (controller == nullptr)
		return;
	UE_LOG(LogTemp, Log, TEXT("MOVING"));
	if (TargetPoints.Num() > 0)
		controller->MoveToActor(TargetPoints[TargetPointIndex]);
}

void AFPSAIGuard::OnArriveAtTargetPoint()
{
	UE_LOG(LogTemp, Log, TEXT("STOPPED"));
	TargetPointIndex++;
	if (TargetPointIndex >= TargetPoints.Num())
		TargetPointIndex = 0;
	MoveToNextTargetPoint();
}