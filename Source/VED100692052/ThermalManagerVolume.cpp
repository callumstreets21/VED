#include "ThermalManagerVolume.h"
#include "ThermalComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

AThermalManagerVolume::AThermalManagerVolume()
{
    PrimaryActorTick.bCanEverTick = false;
    SetActorEnableCollision(true);
}

void AThermalManagerVolume::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        ThermalUpdateTimer,
        this,
        &AThermalManagerVolume::UpdateThermalExchange,
        UpdateInterval,
        true
    );
    
}

void AThermalManagerVolume::UpdateThermalExchange()
{
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors);

    TArray<UThermalComponent*> ThermalComponents;

    for (AActor* Actor : OverlappingActors)
    {
        if (UThermalComponent* Comp = Actor->FindComponentByClass<UThermalComponent>())
        {
            ThermalComponents.Add(Comp);
        }
    }

    const float DeltaTime = UpdateInterval;

    // Unique pairwise thermal exchange
    for (int32 i = 0; i < ThermalComponents.Num(); ++i)
    {
        UThermalComponent* A = ThermalComponents[i];
        for (int32 j = i + 1; j < ThermalComponents.Num(); ++j)
        {
            UThermalComponent* B = ThermalComponents[j];

            float Distance = FVector::Dist(A->GetComponentLocation(), B->GetComponentLocation());
            float MaxRange = FMath::Min(A->InteractionRange, B->InteractionRange);

            if (Distance <= MaxRange)
            {
                float SourceTempA = A->bIsHeatSource ? A->SourceTemperature : A->CurrentTemperature;
                float SourceTempB = B->bIsHeatSource ? B->SourceTemperature : B->CurrentTemperature;

                float TempDifference = SourceTempB - SourceTempA;

                // Heat source strength acts as a multiplier on conductivity
                float StrengthA = A->bIsHeatSource ? A->SourceStrength : 1.0f;
                float StrengthB = B->bIsHeatSource ? B->SourceStrength : 1.0f;
                
                float CombinedConductivity = 0.5f * (A->ThermalConductivity * StrengthA + B->ThermalConductivity * StrengthB);

                float DistanceFactor = 1.0f - (Distance / MaxRange);
                DistanceFactor = FMath::Clamp(DistanceFactor, 0.0f, 1.0f); // Ensure safe bounds

                float HeatTransfer = CombinedConductivity * DistanceFactor * DeltaTime * TempDifference;

                A->AddHeatDelta(HeatTransfer);
                B->AddHeatDelta(-HeatTransfer);
            }
        }
    }

    // Apply the calculated deltas
    for (UThermalComponent* Comp : ThermalComponents)
    {
        Comp->CommitHeatDelta();
    }
}
