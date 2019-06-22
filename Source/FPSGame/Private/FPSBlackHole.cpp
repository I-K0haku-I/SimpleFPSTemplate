// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	DestructionSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("DestructionSphereCom"));
	// DestructionSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	DestructionSphereComp->SetupAttachment(MeshComp);

	DestructionSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::ShredAllApart);

	AttractionSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("AttractionSphereComp"));
	AttractionSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	AttractionSphereComp->SetupAttachment(MeshComp);

}

void AFPSBlackHole::ShredAllApart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)  
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappedComp;
	AttractionSphereComp->GetOverlappingComponents(OverlappedComp);
	for (UPrimitiveComponent* Comp : OverlappedComp)
		if (Comp && Comp->IsSimulatingPhysics())
			Comp->AddForce(((AttractionSphereComp->GetScaledSphereRadius() * (GetActorLocation() - Comp->GetComponentLocation()).GetSafeNormal()) - (GetActorLocation() - Comp->GetComponentLocation())) * SuccStrength);

}