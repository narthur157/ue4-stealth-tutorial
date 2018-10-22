// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Launchpad.generated.h"

class UBoxComponent;

UCLASS()
class FPSGAME_API ALaunchpad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaunchpad();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* LaunchEffect;

	UPROPERTY(EditAnywhere, Category = "Params")
	int32 BoostStrength = 1000;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

public:	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
