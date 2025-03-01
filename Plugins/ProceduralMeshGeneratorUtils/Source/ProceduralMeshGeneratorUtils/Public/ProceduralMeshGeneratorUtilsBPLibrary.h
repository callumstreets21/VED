// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralMeshGeneratorUtilsBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UProceduralMeshGeneratorUtilsBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Sphere Mesh", Keywords = "Procedural spehre utils "), Category = "Utils")
	static void GenerateSphereMesh(UProceduralMeshComponent* Mesh, float Radius, int LatitudeBands, int LongitudeBands);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Cube Mesh", Keywords = "Procedural cube utils "), Category = "Utils")
	static void GenerateCubeMesh(UProceduralMeshComponent* Mesh, float SideLength);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Generate Disk Mesh", Keywords = "Procedural disk utils "), Category = "Utils")
	static void GenerateBeveledDisk(UProceduralMeshComponent* Mesh,
	float InnerRadius = 50.0f,
	float OuterRadius = 100.0f,
	int32 RadialSegments = 32,
	int32 TubularSegments = 16);
};
