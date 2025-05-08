// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProfilingToolsPlusEditorMode.h"
#include "ProfilingToolsPlusEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "ProfilingToolsPlusEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/ProfilingToolsPlusSimpleTool.h"
#include "Tools/ProfilingToolsPlusInteractiveTool.h"

// step 2: register a ToolBuilder in FProfilingToolsPlusEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "ProfilingToolsPlusEditorMode"

const FEditorModeID UProfilingToolsPlusEditorMode::EM_ProfilingToolsPlusEditorModeId = TEXT("EM_ProfilingToolsPlusEditorMode");

FString UProfilingToolsPlusEditorMode::SimpleToolName = TEXT("ProfilingToolsPlus_ActorInfoTool");
FString UProfilingToolsPlusEditorMode::InteractiveToolName = TEXT("ProfilingToolsPlus_MeasureDistanceTool");


UProfilingToolsPlusEditorMode::UProfilingToolsPlusEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UProfilingToolsPlusEditorMode::EM_ProfilingToolsPlusEditorModeId,
		LOCTEXT("ModeName", "ProfilingToolsPlus"),
		FSlateIcon(),
		true);
}


UProfilingToolsPlusEditorMode::~UProfilingToolsPlusEditorMode()
{
}


void UProfilingToolsPlusEditorMode::ActorSelectionChangeNotify()
{
}

void UProfilingToolsPlusEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FProfilingToolsPlusEditorModeCommands& SampleToolCommands = FProfilingToolsPlusEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UProfilingToolsPlusSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UProfilingToolsPlusInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UProfilingToolsPlusEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FProfilingToolsPlusEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UProfilingToolsPlusEditorMode::GetModeCommands() const
{
	return FProfilingToolsPlusEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
