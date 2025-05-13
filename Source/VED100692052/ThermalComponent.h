// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThermalComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VED100692052_API UThermalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UThermalComponent();

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThermalIgnitionStartEvent, UThermalComponent*, SourceComponent);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FThermalIgnitionEndEvent, UThermalComponent*, SourceComponent);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal")
    float CurrentTemperature = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal")
    float HeatCapacity = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal")
    float ThermalConductivity = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thermal")
    float InteractionRange = 1000.0f;

    // Temperature the object will cool toward
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Ambient")
    float AmbientTemperature = 0.0f;

    // Rate of temperature loss per second (e.g., 0.1 = slow loss, 1.0 = fast loss)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Ambient")
    float AmbientLossRate = 0.0005f;

    // Whether this component emits constant heat
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Heat Source")
    bool bIsHeatSource = false;

    // Temperature it maintains while radiating (acts like a hot plate)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Heat Source", meta=(EditCondition="bIsHeatSource"))
    float SourceTemperature = 100.0f;

    // How strong the heat radiation is (acts like conductivity * multiplier)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Heat Source", meta=(EditCondition="bIsHeatSource"))
    float SourceStrength = 1.0f;

    // --- Burning Properties ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Burning")
    bool bCanBurn = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Burning", meta=(EditCondition="bCanBurn"))
    bool bIsBurning = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Burning", meta=(EditCondition="bCanBurn"))
    float IgnitionTemperature = 350.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Burning", meta=(EditCondition="bCanBurn"))
    float BurnDuration = 10.0f; // seconds

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Thermal|Burning", meta=(EditCondition="bCanBurn"))
    float BurnHeatOutput = 1500.0f; // heat per second
    
    float BurnTimeRemaining = 0.0f;

    // Fired when this component ignites
    UPROPERTY(BlueprintAssignable, Category="Thermal|Events")
    FThermalIgnitionStartEvent OnIgnited;

    UPROPERTY(BlueprintAssignable, Category="Thermal|Events")
    FThermalIgnitionEndEvent OnIgnitionEnded;
    
    void AddHeatDelta(float Delta);
    void CommitHeatDelta();
    FVector GetComponentLocation() const;

protected:
    virtual void BeginPlay() override;

private:
    float PendingHeatDelta = 0.0f;

		
};
