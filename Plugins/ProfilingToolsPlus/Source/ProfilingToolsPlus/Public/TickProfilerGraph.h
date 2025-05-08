// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROFILINGTOOLSPLUS_API STickProfilerGraph : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STickProfilerGraph) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		// Optional UI layout here
	}

	void SetTickTimes(const TMap<AActor*, double>& InTickTimes)
	{
		TickTimes = InTickTimes;
	}
	bool bShouldDraw = false;
protected:
	virtual int32 OnPaint(    const FPaintArgs& Args,
	const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements,
	int32 LayerId,
	const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const override;

private:
	TMap<AActor*, double> TickTimes;
};