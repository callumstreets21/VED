// Fill out your copyright notice in the Description page of Project Settings.
#include "CoreProfilingSubsystem.h"
#include <ProfilingDebugging/ScopedTimers.h>




bool bWaitingForBeginPlay;
bool bAreAllActorsInitialized = true;




void UCoreProfilingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Warning, TEXT("Profiler Initialisation Beginning"));
	Super::Initialize(Collection);

	LoadBlacklist();
	// World has already started, call AutoInject immediately
	bWaitingForBeginPlay = true;
	
	FWorldDelegates::OnWorldPostActorTick.AddUObject(this, &UCoreProfilingSubsystem::OnPostActorTick);

}

void UCoreProfilingSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("Profiler Denitialisation Beginning"));
	SaveBlacklist();
	bAreAllActorsInitialized = false;
	Super::Deinitialize();
}

void UCoreProfilingSubsystem::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	
	CleanupInvalidActors();
	UWorld* PIEWorld = GetPIEWorld();
	if (!PIEWorld)
	{
		return;
	}
	if (bWaitingForBeginPlay)
	{
		
		bWaitingForBeginPlay = false;
	}
	if(bEnableVisualization) {
		VisualizeTickData();
	}

	

	
	
}


void UCoreProfilingSubsystem::OnPostActorTick(UWorld* World, ELevelTick TickType, float DeltaSeconds)
{
	if (World && World->WorldType == EWorldType::PIE)
	{
		bAreAllActorsInitialized = true;
		
		for (AActor* Actor : World->GetCurrentLevel()->Actors)
		{
			if (Actor && !Actor->HasActorBegunPlay())
			{
				bAreAllActorsInitialized = false;
				break;
			}
		}

		if (bAreAllActorsInitialized)
		{
			UE_LOG(LogTemp, Log, TEXT("All actors have started their BeginPlay in PIE session."));
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &UCoreProfilingSubsystem::ReinjectProfilingComponents, 0.2, false);

			//ReinjectProfilingComponents();
			FWorldDelegates::OnWorldPostActorTick.RemoveAll(this);
		}
	}
}


bool UCoreProfilingSubsystem::CheckIfBlacklistedClass(AActor* Actor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Entries in blacklist : %d"), Blacklist.Num());
	if (!Actor)
	{
		//UE_LOG(LogTemp, Warning, TEXT("CheckIfBlacklistedClass: Null actor passed in"));
		return false;
	}

	for (UClass* SkipClass : Blacklist)
	{
		if (!SkipClass)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Found null entry in Blacklist"));
			continue;
		}
        
		// Using GetClass()->IsChildOf() can sometimes be more explicit.
		if (Actor->GetClass()->IsChildOf(SkipClass) || Actor->IsA(SkipClass))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Actor %s of class %s is blacklisted by %s"),
				//*Actor->GetName(), *Actor->GetClass()->GetName(), *SkipClass->GetName());
			return true;
		}
		else
		{
			//UE_LOG(LogTemp, Verbose, TEXT("Actor %s of class %s is NOT a child of %s"),
				//*Actor->GetName(), *Actor->GetClass()->GetName(), *SkipClass->GetName());
		}
	}

	return false;
}

void UCoreProfilingSubsystem::AutoInjectProfilingComponent()
{

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!IsValid(Actor) || CheckIfBlacklistedClass(Actor) || !Actor->IsActorTickEnabled()) continue;

		FProfilingTickFunction* ProfilingTick = new FProfilingTickFunction();
		ProfilingTick->OwningActor = Actor;
		ProfilingTick->Target = Actor;
		ProfilingTick->bCanEverTick = true;
		ProfilingTick->bStartWithTickEnabled = true;
		ProfilingTick->TickGroup = TG_PrePhysics;
		ProfilingTick->OnTickMeasured = [this](AActor* MeasuredActor, double Time) {
			ActorTickDurations.FindOrAdd(MeasuredActor) = Time;
		};

		// Initialize and register with the world
		ProfilingTick->RegisterTickFunction(GetWorld()->GetCurrentLevel());
		ProfilingTickFunctions.Add(Actor, ProfilingTick);

		// Optionally disable default actor tick to prevent double ticking
		Actor->PrimaryActorTick.SetTickFunctionEnable(false);
	}
	
}

void UCoreProfilingSubsystem::RemoveProfilingComponents()
{
	UE_LOG(LogTemp, Warning, TEXT("Removing profiling components from actors"));
	for (auto& Pair : ProfilingTickFunctions)
	{
		AActor* Actor = Pair.Key;
		FProfilingTickFunction* TickFunc = Pair.Value;
		
		if (TickFunc)
		{
			TickFunc->SetTickFunctionEnable(false);
			TickFunc->UnRegisterTickFunction();
			
			delete TickFunc;
		}

		if (IsValid(Actor))
		{
			Actor->PrimaryActorTick.SetTickFunctionEnable(true); // Restore default
			UE_LOG(LogTemp, Warning, TEXT("Removed profiling component from actor: %s"), *Actor->GetName());
		}
		
	}
	ActorTickDurations.Empty();
	ProfilingTickFunctions.Empty();
}

TStatId UCoreProfilingSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UCoreProfilingSubsystem, STATGROUP_Tickables);
}

void UCoreProfilingSubsystem::ReportActorTick(AActor* Actor, float TickDuration)
{
	if (!Actor) return;

	FActorTickSample& Sample = TickData.FindOrAdd(Actor);
	Sample.LastTickTime = TickDuration;
	Sample.SmoothedTime = FMath::Lerp(Sample.SmoothedTime, TickDuration, 0.1f);
}

void UCoreProfilingSubsystem::ReinjectProfilingComponents()
{
	SaveBlacklist();
	RemoveProfilingComponents();
	if(bEnableProfiling) AutoInjectProfilingComponent();
}

void UCoreProfilingSubsystem::CleanupInvalidActors()
{
	for (auto It = TickData.CreateIterator(); It; ++It)
	{
		if (!It->Key.IsValid())
			It.RemoveCurrent();
	}
}

void UCoreProfilingSubsystem::SetEnableVisualization(bool bEnable)
{
	bEnableVisualization = bEnable;
	if (bEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Visualization enabled"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Visualization disabled"));
		
	}
}

void UCoreProfilingSubsystem::SetEnableProfiling(bool bEnable)
{
	bEnableProfiling = bEnable;
	if (bEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Profiling enabled"));
		AutoInjectProfilingComponent();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Profiling disabled"));
		RemoveProfilingComponents();
	}
}

void UCoreProfilingSubsystem::VisualizeTickData()
{
	longestTickLastFrame = 0.0f;
	totalActorTickTimeLastFrame = 0.0f;
	for (const auto& Pair : ActorTickDurations)
	{
		AActor* Actor = Pair.Key;
		double TickTimeSeconds = Pair.Value;

		if (IsValid(Actor) && !CheckIfBlacklistedClass(Actor))
		{
			totalActorTickTimeLastFrame += TickTimeSeconds;
			FString DebugText = FString::Printf(TEXT("%s: %.2f ms"), *Actor->GetName(), TickTimeSeconds * 1000.0);
			FVector TextLocation = Actor->GetActorLocation() + FVector(0, 0, 100); // float above actor
			DrawDebugString(GetWorld(), TextLocation, DebugText, nullptr, FColor::Green, 0.f, false);
			if (TickTimeSeconds > longestTickLastFrame)
			{
				longestTickLastFrame = TickTimeSeconds;
				longestTickActorName = Actor->GetName();
			}
		}
	}
}

void UCoreProfilingSubsystem::SaveBlacklist()
{
	FString SectionName = TEXT("CoreProfilingSubsystem");
	FString ConfigPath = GGameIni;

	// Validate the Blacklist
	TArray<UClass*> ValidBlacklist;
	for (UClass* Class : Blacklist)
	{
		if (Class && !Class->HasAnyClassFlags(CLASS_Transient)) // Exclude null and transient classes
		{
			GConfig->SetString(*SectionName, *Class->GetName(), *Class->GetPathName(), ConfigPath);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid or transient class found in Blacklist, skipping."));
		}
	}

	GConfig->Flush(false, ConfigPath);
	UE_LOG(LogTemp, Log, TEXT("Blacklist saved to config."));
}

void UCoreProfilingSubsystem::LoadBlacklist()
{
	FString SectionName = TEXT("CoreProfilingSubsystem");
	FString ConfigPath = GGameIni;

	TArray<FString> ClassNames;
	GConfig->GetSection(*SectionName, ClassNames, ConfigPath);

	Blacklist.Empty();

	UE_LOG(LogTemp, Warning, TEXT("Loading Blacklist from config. Found %d classes."), ClassNames.Num());
	for (const FString& ClassEntry : ClassNames)
	{
		// Split the string at '=' to extract the value (path)
		FString Key, ClassPath;
		if (ClassEntry.Split(TEXT("="), &Key, &ClassPath))
		{
			UE_LOG(LogTemp, Warning, TEXT("Loading class: %s"), *ClassPath);

			// Use the extracted path to find the class
			UClass* Class = FindObject<UClass>(nullptr, *ClassPath);
			if (Class)
			{
				Blacklist.Add(Class);
				UE_LOG(LogTemp, Warning, TEXT("Successfully found and added class: %s"), *ClassPath);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to find class: %s"), *ClassPath);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid entry in config: %s"), *ClassEntry);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Blacklist loaded from config."));
}

void UCoreProfilingSubsystem::CopySubsystemsSettingsTo(UCoreProfilingSubsystem* TargetSubsystem)
{
	if (!TargetSubsystem) return;

	TargetSubsystem->bEnableVisualization = bEnableVisualization;
	TargetSubsystem->Blacklist = Blacklist;
	TargetSubsystem->bEnableProfiling = bEnableProfiling;
	UE_LOG(LogTemp, Warning, TEXT("Copied blacklist to target subsystem. Entries: %d"), Blacklist.Num());
}

void UCoreProfilingSubsystem::CopySubsystemSettingsFrom(const UCoreProfilingSubsystem* SourceSubsystem)
{
	if (!SourceSubsystem) return;
	bEnableVisualization = SourceSubsystem->bEnableVisualization;
	bEnableProfiling = SourceSubsystem->bEnableProfiling;
	Blacklist = SourceSubsystem->Blacklist;

	UE_LOG(LogTemp, Warning, TEXT("Copied blacklist from source subsystem. Entries: %d"), Blacklist.Num());
}