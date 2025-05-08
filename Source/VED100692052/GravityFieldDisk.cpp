// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityFieldDisk.h"

#include "GeneratorFunctions.h"

void AGravityFieldDisk::ComputeGravity(FVector Positon, FVector& Gravity)
{
    FVector PosInDiskSpace = Positon;
    PosInDiskSpace = ConvertPosToDiskLocalspace(Positon);

    FVector PosXYPlane = PosInDiskSpace;
    PosXYPlane.Z = 0;

    if (PosXYPlane.Length() <= (innerRadius+outerRadius)/2) {
        FVector Direction = -1*(GetActorUpVector() * PosInDiskSpace.Z);
        Direction.Normalize();
        Gravity = Direction;
        return;
    }
    else {
        FVector outerGravityOriginXY = PosXYPlane;
        outerGravityOriginXY.Normalize();
        outerGravityOriginXY *= (innerRadius + outerRadius) / 2;

        Gravity = (outerGravityOriginXY - PosInDiskSpace);
        Gravity.Normalize();
        return;
    }
    
}

FVector AGravityFieldDisk::ConvertPosToDiskLocalspace(const FVector& GlobalPosition) const
{
    // Get the transform of the GravityFieldDisk
    FTransform DiskTransform = RootComponent->GetComponentTransform();

    // Convert the global position to local space
    FVector LocalPosition = DiskTransform.InverseTransformPosition(GlobalPosition);


    return LocalPosition;
}

FVector AGravityFieldDisk::ConvertPosToGlobalSpace(const FVector& LocalPosition) const
{
    // Get the transform of the GravityFieldDisk
    FTransform DiskTransform = RootComponent->GetComponentTransform();

    // Convert the local position to global space
    FVector GlobalPosition = DiskTransform.TransformPosition(LocalPosition);

    return GlobalPosition;
}