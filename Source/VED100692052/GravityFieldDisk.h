// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GravityFieldBase.h"
#include "GravityFieldDisk.generated.h"

/**
 * 
 */
UCLASS()
class VED100692052_API AGravityFieldDisk : public AGravityFieldBase
{
	GENERATED_BODY()
public:
	virtual void ComputeGravity(FVector Positon, FVector& Gravity) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvertGravity = false;
	
};
