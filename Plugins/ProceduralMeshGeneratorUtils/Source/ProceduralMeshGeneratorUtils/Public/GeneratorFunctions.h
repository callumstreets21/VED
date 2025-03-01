// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"

/**
 * 
 */
class PROCEDURALMESHGENERATORUTILS_API GeneratorFunctions
{
public:
	GeneratorFunctions();
	~GeneratorFunctions();

	static void GenerateSphereMesh(UProceduralMeshComponent* Mesh, float Radius, int LatitudeBands, int LongitudeBands);

	static void GenerateBeveledDisk(UProceduralMeshComponent* Mesh,
	float InnerRadius = 50.0f,
	float OuterRadius = 100.0f,
	int32 RadialSegments = 32,
	int32 TubularSegments = 16);

};
