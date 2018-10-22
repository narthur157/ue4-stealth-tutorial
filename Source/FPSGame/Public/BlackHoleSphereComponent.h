// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "BlackHoleSphereComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPSGAME_API UBlackHoleSphereComponent : public USphereComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UBlackHoleSphereComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	int32 PullRadius = 100000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Field")
	int32 PullStrength = -100000;
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
