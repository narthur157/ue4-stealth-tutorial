#include "BlackHoleSphereComponent.h"


// Sets default values for this component's properties
UBlackHoleSphereComponent::UBlackHoleSphereComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UBlackHoleSphereComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UBlackHoleSphereComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TSet<UPrimitiveComponent *> Comps;

	GetOverlappingComponents(Comps);
	for (UPrimitiveComponent* Comp : Comps)
	{
		Comp->AddRadialForce(GetOwner()->GetActorLocation(), PullRadius, PullStrength, ERadialImpulseFalloff::RIF_Linear, true);
	}

}
