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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool bInvertGravity = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	float innerRadius = 50;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	float outerRadius = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	int radialSegments = 16;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	int tubularSegmenets = 16;

	
};
