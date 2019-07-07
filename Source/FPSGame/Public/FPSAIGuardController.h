// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "FPSAIGuardController.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIGuardController : public AAIController
{
	GENERATED_BODY()

public:
	AFPSAIGuardController();

protected:
	// virtual void BeginPlay() override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
