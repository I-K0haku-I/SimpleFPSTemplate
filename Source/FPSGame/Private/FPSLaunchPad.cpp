// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "FPSCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetWorldScale3D(FVector(2.f, 2.f, 0.2f));
	RootComponent = MeshComp;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	OverlapComp->SetWorldLocation(FVector(0.f, 0.f, 100.f));
	OverlapComp->SetBoxExtent(FVector(40.f, 40.f, 50.f));
	OverlapComp->SetupAttachment(RootComponent);

	LaunchDegree = 45.f;
	LaunchForce = 1000.f;

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleLaunch);
}

void AFPSLaunchPad::PlayLaunchEffect()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation());
}

void AFPSLaunchPad::HandleLaunch(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ASDLJKASKLDJKIKOLALKSDHJJAK"));
	FRotator PadRot = GetActorRotation();
	PadRot.Pitch += LaunchDegree;
	FVector LaunchVec = PadRot.Vector() * LaunchForce;

	ACharacter* Char = Cast<ACharacter>(OtherActor);
	if (Char)
	{
		Char->LaunchCharacter(LaunchVec, true, true);
		PlayLaunchEffect();
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->SetAllPhysicsLinearVelocity(FVector(0.f));
		OtherComp->AddImpulse(LaunchVec, NAME_None, true);
		PlayLaunchEffect();
	}
}