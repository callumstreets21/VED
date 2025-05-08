// Fill out your copyright notice in the Description page of Project Settings.


#include "ProfilingComponent.h"
#include "CoreProfilingSubsystem.h"

// Sets default values for this component's properties
UProfilingComponent::UProfilingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called every frame
void UProfilingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!GetOwner()) return;

	// This tick will wrap the actor's tick due to prerequisites
	if (TickStart == 0)
	{
		PreActorTick(DeltaTime);
	}
	else
	{
		PostActorTick(DeltaTime);
		TickStart = 0;
	}
}

// Called when the game starts
void UProfilingComponent::BeginPlay()
{
	
	Super::BeginPlay();
	if (AActor* Owner = GetOwner())
	{
		
		if (Owner->PrimaryActorTick.IsTickFunctionEnabled() && PrimaryComponentTick.IsTickFunctionEnabled())
		{
			// These are safe to call even if already set
			Owner->PrimaryActorTick.AddPrerequisite(this, PrimaryComponentTick);
			UE_LOG(LogTemp, Warning, TEXT("Hooked tick for actor: %s"), *Owner->GetName());
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Tick functions disabled, removing component: %s"), *Owner->GetName());
			GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &UProfilingComponent::SelfDestruct, 0.1, false);
		}
	}
	
}


void UProfilingComponent::SelfDestruct()
{
	DestroyComponent();
}


void UProfilingComponent::PreActorTick(float DeltaSeconds)
{
	TickStart = FPlatformTime::Seconds();
}

void UProfilingComponent::PostActorTick(float DeltaSeconds)
{
	double End = FPlatformTime::Seconds();
	float Duration = static_cast<float>(End - TickStart);

	if (UWorld* World = GetWorld())
	{
		if (UCoreProfilingSubsystem* Subsystem = World->GetSubsystem<UCoreProfilingSubsystem>())
		{
			Subsystem->ReportActorTick(GetOwner(), Duration);
		}
	}
}


