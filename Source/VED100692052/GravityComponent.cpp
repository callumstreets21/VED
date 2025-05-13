#include "GravityComponent.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"

UGravityComponent::UGravityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGravityComponent::BeginPlay()
{
	Super::BeginPlay();

	TargetPrimitive = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	if (TargetPrimitive && bOverrideWorldGravity)
	{
		TargetPrimitive->SetEnableGravity(false);
	}
}

void UGravityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
	

	if (!TargetPrimitive || !TargetPrimitive->IsSimulatingPhysics())
		return;

	FVector Force = GravityDirection.GetSafeNormal() * GravityStrength * TargetPrimitive->GetMass();
	TargetPrimitive->AddForce(Force);
}
