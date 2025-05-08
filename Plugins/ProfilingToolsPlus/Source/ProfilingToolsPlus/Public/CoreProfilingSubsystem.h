// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EngineUtils.h"
#include "GameplayDebuggerPlayerManager.h"
#include "NavigationData.h"
#include "Kismet/GameplayStatics.h"
#include "ProfilingComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "GameFramework/HUD.h"
#include "Misc/FileHelper.h"
#include "ProfilingTickFunction.h"
#include "Misc/Paths.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CoreProfilingSubsystem.generated.h"


/**
 * 
 */



USTRUCT()
struct FActorTickSample
{
	GENERATED_BODY()

	float LastTickTime = 0.f;
	float SmoothedTime = 0.f;
};

UCLASS()
class PROFILINGTOOLSPLUS_API UCoreProfilingSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:

	TMap<AActor*, FProfilingTickFunction*> ProfilingTickFunctions;

	TMap<AActor*, double> ActorTickDurations;

	// Initialize the subsystem, hook up overlap events
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Cleanup any events when the subsystem is deinitialized
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override
	{
		return true;
	}
	
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override
	{
		return true;
	}
		void ReportActorTick(AActor* Actor, float TickDuration);
	TArray<UClass*>& GetBlacklist() { return Blacklist; }

	void ReinjectProfilingComponents();
	void CopySubsystemsSettingsTo(UCoreProfilingSubsystem* TargetSubsystem);
	void CopySubsystemSettingsFrom(const UCoreProfilingSubsystem* SourceSubsystem);
	// Add a setter for the boolean that also updates the visualization
	void SetEnableVisualization(bool bEnable);
	void SetEnableProfiling(bool bEnable);
	bool GetEnableVisualization() {return bEnableVisualization;};
	bool GetEnableProfiling() {return bEnableProfiling;};

	float GetLongestTickLastFrame() { return longestTickLastFrame; }
	float GetTotalTickLastFrame() { return longestTickLastFrame; }
	FString GetLongestTickActorName() { return longestTickActorName; }

protected:
	UPROPERTY(BlueprintReadWrite, Category="Profiling")
	TArray<UClass*> Blacklist;

	
	FTimerHandle DelayHandle;
	void OnPostActorTick(UWorld* World, ELevelTick TickType, float DeltaSeconds);


	virtual void Tick(float DeltaTime);
	bool CheckIfBlacklistedClass(AActor* Actor);
	void AutoInjectProfilingComponent();
	void RemoveProfilingComponents();
	virtual TStatId GetStatId() const override;

	void CleanupInvalidActors();
	bool IsTickable() const override { return true; }
	bool IsTickableInEditor() const override { return false; } // Optional
	bool IsTickableWhenPaused() const override { return false; }


	
private:

	float longestTickLastFrame;
	float totalActorTickTimeLastFrame;
	FString longestTickActorName;
	TMap<TWeakObjectPtr<AActor>, FActorTickSample> TickData;
	void VisualizeTickData();
	void SaveBlacklist();
	void LoadBlacklist();

	UPROPERTY(EditAnywhere, Category = "Profiling")
	bool bEnableVisualization = false;

	UPROPERTY(EditAnywhere, Category = "Profiling")
	bool bEnableProfiling = false;
};


inline UWorld* GetPIEWorld()
{
	if (!GEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine is null!"));
		return nullptr;
	}

	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (Context.WorldType == EWorldType::PIE)
		{
			return Context.World();
		}
	}
	
	return nullptr;
}

inline UWorld* GetEditorWorld()
{
	if (!GEngine)
	{
		UE_LOG(LogTemp, Error, TEXT("GEngine is null!"));
		return nullptr;
	}
	for (const FWorldContext& Context : GEngine->GetWorldContexts())
	{
		if (Context.WorldType == EWorldType::Editor)
		{
			return Context.World();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No Editor world found!"));
	return nullptr;
}