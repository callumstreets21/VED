// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorFunctions.h"

GeneratorFunctions::GeneratorFunctions()
{
}

GeneratorFunctions::~GeneratorFunctions()
{
}

struct FMeshData {
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
};

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

void GeneratorFunctions::GenerateBeveledDisk(UProceduralMeshComponent* Mesh,
    float InnerRadius,
    float OuterRadius,
    int32 RadialSegments,
    int32 TubularSegments)
{

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    Vertices.Empty();
    Triangles.Empty();
    Normals.Empty();
    UVs.Empty();
    Tangents.Empty();

    float TwoPi = 2.0f * PI;
    float SegmentAngleStep = TwoPi / RadialSegments;
    float TubeAngleStep = PI / TubularSegments;  // Only 180 degrees for half-circle
    float TorusRadius = (OuterRadius - InnerRadius) * 0.5f;
    float CenterRadius = InnerRadius + TorusRadius;

    // Generate vertices for outer half-circle segments
    for (int32 RadialIndex = 0; RadialIndex <= RadialSegments; RadialIndex++)
    {
        float Theta = RadialIndex * SegmentAngleStep;
        FVector RingCenter = FVector(FMath::Cos(Theta) * CenterRadius, FMath::Sin(Theta) * CenterRadius, 0);

        for (int32 TubularIndex = 0; TubularIndex <= TubularSegments; TubularIndex++)
        {
            float Phi = (TubularIndex * TubeAngleStep) - (PI / 2);  // Shift by -90° to center it

            // Generate only the outer half of the torus tube
            FVector LocalPos = FVector(FMath::Cos(Phi) * TorusRadius, 0, FMath::Sin(Phi) * TorusRadius);

            // Rotate LocalPos around Z-axis by Theta
            FVector Vertex = RingCenter + FVector(
                LocalPos.X * FMath::Cos(Theta) - LocalPos.Y * FMath::Sin(Theta),
                LocalPos.X * FMath::Sin(Theta) + LocalPos.Y * FMath::Cos(Theta),
                LocalPos.Z
            );

            Vertices.Add(Vertex);

            // Compute Normal (pointing outward)
            FVector Normal = (Vertex - RingCenter).GetSafeNormal();
            Normals.Add(Normal);

            // Compute UV
            UVs.Add(FVector2D((float)RadialIndex / RadialSegments, (float)TubularIndex / TubularSegments));

            // Compute Tangent (aligned with the ring)
            FVector TangentX = FVector(-FMath::Sin(Theta), FMath::Cos(Theta), 0);
            Tangents.Add(FProcMeshTangent(TangentX, false));
        }
    }

    // Generate triangles for the half-circle torus
    for (int32 RadialIndex = 0; RadialIndex < RadialSegments; RadialIndex++)
    {
        for (int32 TubularIndex = 0; TubularIndex < TubularSegments; TubularIndex++)
        {
            int32 A = (RadialIndex * (TubularSegments + 1)) + TubularIndex;
            int32 B = A + (TubularSegments + 1);
            int32 C = B + 1;
            int32 D = A + 1;

            Triangles.Add(A);
            Triangles.Add(B);
            Triangles.Add(C);

            Triangles.Add(A);
            Triangles.Add(C);
            Triangles.Add(D);
        }
    }


    Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
    Mesh->AddCollisionConvexMesh(Vertices);
}
