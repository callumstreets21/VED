// Fill out your copyright notice in the Description page of Project Settings.


// CustomTickFunction.cpp
#include "ProfilingTickFunction.h"
#include "HAL/PlatformTime.h"

void FProfilingTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent)
{
	double Start = FPlatformTime::Seconds();

	if (Target)
	{
		Target->TickActor(DeltaTime, TickType, *this);
	}

	double Elapsed = FPlatformTime::Seconds() - Start;

	if (OnTickMeasured && OwningActor)
	{
		OnTickMeasured(OwningActor, Elapsed);
	}
}

FString FProfilingTickFunction::DiagnosticMessage()
{
	return FString::Printf(TEXT("FProfilingTickFunction (%s)"), *GetNameSafe(OwningActor));
}
