// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityFieldBase.h"
#include "MyGravityFieldBoxParallel.generated.h"

/**
 * 
 */
UCLASS()
class VED100692052_API AMyGravityFieldBoxParallel : public AGravityFieldBase
{
	GENERATED_BODY()
public:
	virtual void ComputeGravity(FVector Positon, FVector& Gravity) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	float SideLength = 100;
};
