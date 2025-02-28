// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityFieldBase.h"


// Sets default values
AGravityFieldBase::AGravityFieldBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	proceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	proceduralMesh->bUseComplexAsSimpleCollision = false;
	RootComponent = proceduralMesh;

}

void AGravityFieldBase::AngleBetweenVectors(FVector V1, FVector V2, double& Angle)
{
	// Normalize the vectors to get the correct angle
	FVector NormalizedV1 = V1.GetSafeNormal();
	FVector NormalizedV2 = V2.GetSafeNormal();

	// Compute the dot product
	float Dot = FVector::DotProduct(NormalizedV1, NormalizedV2);

	// Clamp the dot product to avoid NaN due to floating-point precision
	Dot = FMath::Clamp(Dot, -1.0f, 1.0f);

	// Compute the angle in Radians
	float AngleRadians = FMath::Acos(Dot);

	// Convert to Degrees (if needed)
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	Angle = AngleDegrees;
}

// Called when the game starts or when spawned
void AGravityFieldBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGravityFieldBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGravityFieldBase::ComputeGravity(FVector Positon, FVector& Gravity)
{
}

