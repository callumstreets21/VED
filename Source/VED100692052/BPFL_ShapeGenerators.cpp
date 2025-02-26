// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFL_ShapeGenerators.h"

TArray<FVector> UBPFL_ShapeGenerators::GenerateCubeVertexData(float sideLength)
{
	float halfSide = sideLength / 2.0f;
	TArray<FVector> vertices = {{
		{-halfSide, -halfSide, -halfSide},  // 0
		{ halfSide, -halfSide, -halfSide},  // 1
		{ halfSide,  halfSide, -halfSide},  // 2
		{-halfSide,  halfSide, -halfSide},  // 3
		{-halfSide, -halfSide,  halfSide},  // 4
		{ halfSide, -halfSide,  halfSide},  // 5
		{ halfSide,  halfSide,  halfSide},  // 6
		{-halfSide,  halfSide,  halfSide}   // 7
	}};
	return vertices;
}

FVector UBPFL_ShapeGenerators::checkCubeSideByAngle(float x, float y, float z)
{
	FVector cubeSide = FVector(0.0,0.0,0.0);
	//if out of x bounds
	if( x < 45 || x > 135)
	{
		cubeSide.X = 1.0f;
	}
	if( y < 45 || y > 135)
	{
		cubeSide.Y = 1.0f;
	}
	if( z < 45 || z > 135)
	{
		cubeSide.Z = 1.0f;
	};
	return cubeSide;
}

void UBPFL_ShapeGenerators::GenerateSphereMesh(UProceduralMeshComponent* Mesh, float Radius, int LatitudeBands, int LongitudeBands)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    // Generate vertices
    for (int lat = 0; lat <= LatitudeBands; ++lat)
    {
        float theta = lat * PI / LatitudeBands; // Latitude angle
        float sinTheta = FMath::Sin(theta);
        float cosTheta = FMath::Cos(theta);

        for (int lon = 0; lon <= LongitudeBands; ++lon)
        {
            float phi = lon * 2 * PI / LongitudeBands; // Longitude angle
            float sinPhi = FMath::Sin(phi);
            float cosPhi = FMath::Cos(phi);

            // Compute vertex position
            FVector Vertex = FVector(
                Radius * cosPhi * sinTheta,  // X
                Radius * sinPhi * sinTheta,  // Y
                Radius * cosTheta           // Z
            );

            // Compute normal (normalized position)
            FVector Normal = Vertex.GetSafeNormal();

            // Compute UVs
            FVector2D UV = FVector2D(
                1.0f - (float)lon / LongitudeBands, // U
                1.0f - (float)lat / LatitudeBands  // V
            );

            // Store values
            Vertices.Add(Vertex);
            Normals.Add(Normal);
            UVs.Add(UV);
            VertexColors.Add(FColor::White); // Optional
            Tangents.Add(FProcMeshTangent(0, 1, 0)); // Approximate
        }
    }

    // Generate triangle indices
    for (int lat = 0; lat < LatitudeBands; ++lat)
    {
        for (int lon = 0; lon < LongitudeBands; ++lon)
        {
            int first = (lat * (LongitudeBands + 1)) + lon;
            int second = first + LongitudeBands + 1;

            // Two triangles per quad
            Triangles.Add(first);
            Triangles.Add(second);
            Triangles.Add(first + 1);

            Triangles.Add(second);
            Triangles.Add(second + 1);
            Triangles.Add(first + 1);
        }
    }

    // Create the sphere mesh section
    Mesh->CreateMeshSection(
        0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true
    );

	Mesh->AddCollisionConvexMesh(Vertices);
}

