// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "ThermalManagerVolume.generated.h"

/**
 * 
 */
UCLASS()
class VED100692052_API AThermalManagerVolume : public AVolume
{
	GENERATED_BODY()
	
public:
	AThermalManagerVolume();

	UPROPERTY(EditAnywhere, Category="Thermal")
	float UpdateInterval = 0.1f;

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle ThermalUpdateTimer;

	void UpdateThermalExchange();
};
