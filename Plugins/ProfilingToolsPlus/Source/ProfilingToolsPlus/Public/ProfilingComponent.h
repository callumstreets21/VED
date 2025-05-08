// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProfilingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROFILINGTOOLSPLUS_API UProfilingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProfilingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SelfDestruct();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override
	{
		// Call superclass to ensure base functionality is preserved
		Super::OnComponentDestroyed(bDestroyingHierarchy);

		// Perform custom cleanup, e.g., removing tick prerequisites
		if (AActor* Owner = GetOwner())
		{
			Owner->PrimaryActorTick.RemovePrerequisite(this, PrimaryComponentTick);
			PrimaryComponentTick.RemovePrerequisite(Owner, Owner->PrimaryActorTick);
		}

		UE_LOG(LogTemp, Warning, TEXT("ProfilingComponent on actor %s destroyed."), *GetOwner()->GetName());
	}

public:
	FTimerHandle DelayHandle;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PreActorTick(float DeltaSeconds);
	virtual void PostActorTick(float DeltaSeconds);

private:
	double TickStart = 0;
};
