// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:
	UFUNCTION()
	void HandleLaunch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditInstanceOnly, Category = "Settings")
	float LaunchDegree;

	UPROPERTY(EditInstanceOnly, Category = "Settings")
	float LaunchForce;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	UParticleSystem* ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	USoundBase* ExplosionSound;

	void PlayLaunchEffect();
};
