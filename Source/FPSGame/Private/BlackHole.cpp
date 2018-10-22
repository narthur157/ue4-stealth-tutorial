// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BlackHoleSphereComponent.h"
#include "DeleteSphereComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	DeleteSphereComp = CreateDefaultSubobject<UDeleteSphereComponent>(TEXT("DeleteSphereComp"));
	DeleteSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DeleteSphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	DeleteSphereComp->SetupAttachment(MeshComp);

	PullSphereComp = CreateDefaultSubobject<UBlackHoleSphereComponent>(TEXT("PullSphereComp"));
	PullSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PullSphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	PullSphereComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

