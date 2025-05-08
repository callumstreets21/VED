// Fill out your copyright notice in the Description page of Project Settings.


#include "TickProfilerGraph.h"

int32 STickProfilerGraph::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (!bShouldDraw) return 0;
	
	const float GraphHeight = AllottedGeometry.GetLocalSize().Y;
	const float GraphWidth = AllottedGeometry.GetLocalSize().X;

	// 1. Find max tick time
	double MaxTickTime = 0.001; // Prevent divide-by-zero
	for (const auto& Pair : TickTimes)
	{
		MaxTickTime = FMath::Max(MaxTickTime, Pair.Value);
	}
	const double MaxTimeMs = MaxTickTime * 1000.0; // For grid scaling

	// 2. Draw horizontal grid lines every 0.5 ms
	const double GridStepMs = 0.1;
	for (double TimeMs = 0.0; TimeMs <= MaxTimeMs; TimeMs += GridStepMs)
	{
		float Y = GraphHeight - ((TimeMs / MaxTimeMs) * GraphHeight);

		FPaintGeometry LineGeometry = AllottedGeometry.ToPaintGeometry(
			FVector2D(GraphWidth, 1.0f),
			FSlateLayoutTransform(FVector2D(0.0f, Y))
		);

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			LineGeometry,
			FCoreStyle::Get().GetBrush("GenericWhiteBox"),
			ESlateDrawEffect::None,
			FLinearColor(0.25f, 0.25f, 0.25f, 0.5f)
		);

	}

	// 3. Draw vertical grid lines
	const int32 GridSpacingX = 100;
	for (float X = 0; X < GraphWidth; X += GridSpacingX)
	{
		FPaintGeometry LineGeometry = AllottedGeometry.ToPaintGeometry(
			FVector2D(1.0f, GraphHeight),
			FSlateLayoutTransform(FVector2D(X, 0.0f))
		);

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			LineGeometry,
			FCoreStyle::Get().GetBrush("GenericWhiteBox"),
			ESlateDrawEffect::None,
			FLinearColor(0.15f, 0.15f, 0.15f, 0.3f)
		);
	}

	// 4. Draw bars
	
	const int32 NumBars = TickTimes.Num();
    const float BarWidth = 5.0f;
    
    float Spacing = 0.0f;
    if (NumBars > 1)
    {
        Spacing = ((GraphWidth - BarWidth)-100) / (NumBars - 1); // Distribute across full width
    }
    else if (NumBars == 1)
    {
        Spacing = (GraphWidth) / 2.0f; // Center the single bar
    }
	
	float X = 0;
	int BarIndex = 0;
	for (const auto& Pair : TickTimes)
	{
		X = BarIndex* Spacing;
		double TimeMs = Pair.Value * 1000.0;
		float BarHeight = (TimeMs / MaxTimeMs) * GraphHeight;

		FVector2D BarSize(5.0f, BarHeight);
		FVector2D BarPosition(X, GraphHeight - BarHeight);

		FPaintGeometry PaintGeometry = AllottedGeometry.ToPaintGeometry(
			BarSize,
			FSlateLayoutTransform(BarPosition)
		);

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			PaintGeometry,
			FCoreStyle::Get().GetBrush("GenericWhiteBox"),
			ESlateDrawEffect::None,
			FLinearColor::Green
		);
		// Draw label
		float LabelYOffset = (BarIndex % 2 == 0) ? 0.0f : -12.0f;
		FVector2D TextPosition = FVector2D(X, (BarPosition.Y-BarHeight) + LabelYOffset);
		FSlateFontInfo FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", 8); // Small font

		FString ActorName = Pair.Key ? Pair.Key->GetName() : TEXT("Unknown");

		// Optionally abbreviate long names
		FString Time = FString::Printf(TEXT(" %.2f ms"), Pair.Value * 1000.0);
		if (ActorName.Len() > 10)
		{
			ActorName =  ActorName.Left(10) + TEXT("...") + Time;
		}

		FSlateDrawElement::MakeText(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(TextPosition, FVector2D(60.0f, 12.0f)), // Box for text
			FText::FromString(ActorName),
			FontInfo,
			ESlateDrawEffect::None,
			FLinearColor::White
		);
		BarIndex++;
	}

	return LayerId + 1;
}
