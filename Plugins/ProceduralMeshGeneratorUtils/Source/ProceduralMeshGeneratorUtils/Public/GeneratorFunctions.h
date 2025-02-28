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

};
