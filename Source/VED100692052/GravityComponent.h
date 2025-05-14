// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GravityComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VED100692052_API UGravityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGravityComponent();

	// Direction of gravity (normalized internally)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	FVector GravityDirection = FVector(0, 0, -1);

	// Strength of gravity force
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	float GravityStrength = 980.f;

	// If true, disables Unreal’s default gravity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	bool bOverrideWorldGravity = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gravity")
	bool bEnableGravity = true;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPrimitiveComponent* TargetPrimitive = nullptr;
	
};
