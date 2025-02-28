// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorFunctions.h"

GeneratorFunctions::GeneratorFunctions()
{
}

GeneratorFunctions::~GeneratorFunctions()
{
}

void GeneratorFunctions::GenerateSphereMesh(UProceduralMeshComponent* Mesh, float Radius, int LatitudeBands, int LongitudeBands)
{
    if (Mesh == nullptr || LatitudeBands < 1 || LongitudeBands < 1 || Radius < 1)
    {
        return;
    }
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