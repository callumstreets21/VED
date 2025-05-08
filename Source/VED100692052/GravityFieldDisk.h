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



	UFUNCTION(BlueprintCallable)
	FVector ConvertPosToDiskLocalspace(const FVector& GlobalPosition) const;

	UFUNCTION(BlueprintCallable)
	FVector ConvertPosToGlobalSpace(const FVector& LocalPosition) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInvertGravity = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float innerRadius = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float outerRadius = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int radialSegments = 16;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int tubularSegmenets = 16;

	
};
