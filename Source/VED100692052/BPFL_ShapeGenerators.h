// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
#include "BPFL_ShapeGenerators.generated.h"

/**
 * 
 */
UCLASS()
class VED100692052_API UBPFL_ShapeGenerators : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Math")
	static TArray<FVector> GenerateCubeVertexData(float sideLength);


	UFUNCTION(BlueprintCallable, Category = "Math")
	static FVector checkCubeSideByAngle(float x, float y, float z);

	UFUNCTION(BlueprintCallable, Category = "Math")
	static void GenerateSphereMesh(UProceduralMeshComponent* Mesh, float Radius, int LatitudeBands, int LongitudeBands);
};
