// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuardController.h"
#include "FPSAIGuard.h"


AFPSAIGuardController::AFPSAIGuardController()
{

}

void AFPSAIGuardController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{

	if (Result.IsSuccess())// == EPathFollowingResult::Type::Success)
	{
        AFPSAIGuard* character = Cast<AFPSAIGuard>(GetPawn());
        character->OnArriveAtTargetPoint();
	}
} 