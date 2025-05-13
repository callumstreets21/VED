// Fill out your copyright notice in the Description page of Project Settings.


#include "ThermalComponent.h"


UThermalComponent::UThermalComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    bAutoActivate = true;
}

void UThermalComponent::BeginPlay()
{
    Super::BeginPlay();
   
}

void UThermalComponent::AddHeatDelta(float Delta)
{
    PendingHeatDelta += Delta;
}

void UThermalComponent::CommitHeatDelta()
{
    if (FMath::IsNearlyZero(HeatCapacity)) return;

    // Apply incoming heat
    CurrentTemperature += PendingHeatDelta / HeatCapacity;
    PendingHeatDelta = 0.0f;

    // Handle ambient cooling if not a source
    if (!bIsHeatSource)
    {
        float TempDifference = AmbientTemperature - CurrentTemperature;
        float AmbientDelta = TempDifference * AmbientLossRate;
        CurrentTemperature += AmbientDelta;
    }

    if (bIsHeatSource)
    {
        CurrentTemperature = SourceTemperature;
        UE_LOG(LogTemp, Warning, TEXT("HeatSource"));
    }

    // Check for ignition
    if (bCanBurn && !bIsBurning && CurrentTemperature >= IgnitionTemperature)
    {
        bIsBurning = true;
        BurnTimeRemaining = BurnDuration;
        OnIgnited.Broadcast(this);
    }

    // Handle burning
    if (bIsBurning)
    {
        // Emit heat
        AddHeatDelta(BurnHeatOutput); // this is self-emission; neighbors get this through exchange

        // Burn down
        BurnTimeRemaining -= 0.1;
        if (BurnTimeRemaining <= 0.0f)
        {
            bCanBurn = false;
            bIsBurning = false;
            OnIgnitionEnded.Broadcast(this);
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Current Temperature: %f"), CurrentTemperature);
}



FVector UThermalComponent::GetComponentLocation() const
{
    return GetOwner() ? GetOwner()->GetActorLocation() : FVector::ZeroVector;
}