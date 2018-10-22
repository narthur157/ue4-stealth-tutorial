#include "DeleteSphereComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
UDeleteSphereComponent::UDeleteSphereComponent()
{
}

void UDeleteSphereComponent::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UDeleteSphereComponent::OnOverlap);
}

void UDeleteSphereComponent::OnOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->Destroy();
}