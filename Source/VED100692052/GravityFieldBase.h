// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GravityFieldBase.generated.h"

UCLASS()
class VED100692052_API AGravityFieldBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGravityFieldBase();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void AngleBetweenVectors(FVector V1, FVector V2, double& Angle);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UProceduralMeshComponent> proceduralMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	int32 priority = 0;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	virtual void ComputeGravity(FVector Positon, FVector& Gravity);


};
