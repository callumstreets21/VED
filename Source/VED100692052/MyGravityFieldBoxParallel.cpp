// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGravityFieldBoxParallel.h"

void AMyGravityFieldBoxParallel::ComputeGravity(FVector Positon, FVector& Gravity)
{

	Gravity = -1*RootComponent->GetUpVector();
	Gravity.Normalize();

}
