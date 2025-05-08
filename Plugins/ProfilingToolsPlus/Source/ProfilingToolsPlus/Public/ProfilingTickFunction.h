// Fill out your copyright notice in the Description page of Project Settings.
// CustomTickFunction.h
#pragma once


struct PROFILINGTOOLSPLUS_API FProfilingTickFunction : public FActorTickFunction
{
	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;
	virtual FString DiagnosticMessage() override;
    
	AActor* OwningActor = nullptr;
	TFunction<void(AActor*, double)> OnTickMeasured;
};
