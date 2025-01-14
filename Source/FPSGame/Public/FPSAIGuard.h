// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;
class ATargetPoint;
// class FAIRequestID;
// class FPathFollowingResult::Type;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,

	Suspicious,

	Alerted,
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

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume);

	FRotator OriginalRotation;

	UFUNCTION()
	void ResetOrientation();

	FTimerHandle TimerHandler_ResetOrientation;

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIState GuardState;

	UFUNCTION()
	void OnRep_GuardState();

	UFUNCTION()
	void SetGuardState(EAIState NewState);

	UFUNCTION()
	void MoveToNextTargetPoint();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChange(EAIState NewState);

	UPROPERTY(EditAnywhere, Category = "MovementConfig")
	TArray<ATargetPoint*> TargetPoints;

	UPROPERTY(EditAnywhere, Category = "MovementConfig")
	int32 TargetPointIndex;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnArriveAtTargetPoint();

};
