// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AssetRegistry/AssetRegistryModule.h"
#include "DragAndDrop/ActorDragDropOp.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "ClassViewerModule.h"
#include "SClassViewer.h"
#include "CoreMinimal.h"
#include "ClassViewerFilter.h"
#include "CoreProfilingSubsystem.h"
#include "TickProfilerGraph.h"


class FActorClassFilter : public IClassViewerFilter
{
public:
	// Only show classes that are children of AActor, not abstract, etc.
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& Options, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> FilterFuncs) override
	{
		return InClass->IsChildOf(AActor::StaticClass()) &&
			   !InClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists);
	}
    
	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& Options, const TSharedRef<const IUnloadedBlueprintData> InData, TSharedRef<FClassViewerFilterFuncs> FilterFuncs) override
	{
		// Optionally allow unloaded blueprint classes that are children of AActor
		return InData->IsChildOf(AActor::StaticClass());
	}
};

struct FClassItem
{
	UClass* Class;
	FText ClassName;

	// Constructor to initialize class and name
	FClassItem(UClass* InClass)
		: Class(InClass)
		, ClassName(FText::FromString(InClass->GetName()))
	{}
};

class PROFILINGTOOLSPLUS_API STickProfilingWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STickProfilingWindow) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	TSharedRef<ITableRow> GenerateClassRow(TSharedPtr<FClassItem> Item, const TSharedRef<STableViewBase>& OwnerTable);
	TArray<UClass*> GetAllActorClasses();
	//FReply OnClassSelected(const TSharedPtr<UClass> SelectedClass);
	FReply OnClassSelected(UClass* SelectedClass);
	FReply OnAddButtonClicked();
	void RebuildBlacklistRows();
	FReply OnOpenClassSelectorClicked(int32 RowIndex);
	void OnClassSelected(UClass* InClass, int32 RowIndex);
	TSharedRef<SWidget> GenerateBlacklistRow(int32 RowIndex);

	TWeakObjectPtr<UCoreProfilingSubsystem> CoreProfilingSubsystem;
	// The container for dynamically added rows that display the blacklist
	TSharedPtr<SVerticalBox> BlacklistVBox;
	void UpdateCoreProfilingSubsystem();
	TSharedPtr<STextBlock> LongestTickTextBlock;
	TSharedPtr<STextBlock> LongestTickNameTextBlock;

	TSharedRef<STickProfilerGraph> GraphWidget = SNew(STickProfilerGraph);

	
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override
	{
		//UpdateCoreProfilingSubsystem();
		if (CoreProfilingSubsystem.IsValid())
		{
			GraphWidget->bShouldDraw = CoreProfilingSubsystem->GetEnableProfiling();
			GraphWidget->SetTickTimes(CoreProfilingSubsystem->ActorTickDurations);
		}
	}
	
};
