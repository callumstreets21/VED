// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityFieldSphere.h"
#include "GeneratorFunctions.h"


void AGravityFieldSphere::ComputeGravity(FVector Positon, FVector& Gravity)
{
	// Computing the gravity is easy for a sphere. Simply take the vector from the actor to the centre of the gravity sphere
	if (!bInvertGravity)
	{
		FVector GravityDirection = RootComponent->GetComponentLocation() - Positon;
        GravityDirection.Normalize();
		Gravity = GravityDirection;
	}
	else
	{
		FVector GravityDirection = Positon - RootComponent->GetComponentLocation() ;
		GravityDirection.Normalize();
		Gravity = GravityDirection;
	}

}
