// Fill out your copyright notice in the Description page of Project Settings.

#include "Launchpad.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALaunchpad::ALaunchpad()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComp->SetupAttachment(MeshComp);
}

void ALaunchpad::NotifyActorBeginOverlap(AActor* OtherActor)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchEffect, GetActorLocation());
	FVector AngledLaunch = GetActorForwardVector() + GetActorUpVector();
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("LAUNCHING player"));
		Player->LaunchCharacter(AngledLaunch * BoostStrength, true, true);
		return;
	}
	

	TArray<UActorComponent*> CompList = OtherActor->GetComponentsByClass(UPrimitiveComponent::StaticClass());
	for (UActorComponent* AC : CompList)
	{
		UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(AC);
		if (Prim && Prim->IsSimulatingPhysics())
		{
			UE_LOG(LogTemp, Warning, TEXT("LAUNCHING"));
			Prim->AddImpulse(AngledLaunch * BoostStrength, NAME_None, true);
		}
	}
}

