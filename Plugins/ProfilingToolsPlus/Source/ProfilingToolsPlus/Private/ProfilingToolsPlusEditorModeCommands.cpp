// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProfilingToolsPlusEditorModeCommands.h"
#include "ProfilingToolsPlusEditorMode.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "ProfilingToolsPlusEditorModeCommands"

FProfilingToolsPlusEditorModeCommands::FProfilingToolsPlusEditorModeCommands()
	: TCommands<FProfilingToolsPlusEditorModeCommands>("ProfilingToolsPlusEditorMode",
		NSLOCTEXT("ProfilingToolsPlusEditorMode", "ProfilingToolsPlusEditorModeCommands", "ProfilingToolsPlus Editor Mode"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
{
	
}

void FProfilingToolsPlusEditorModeCommands::RegisterCommands()
{
	TArray <TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd(NAME_Default);

	UI_COMMAND(SimpleTool, "Show Actor Info", "Opens message box with info about a clicked actor", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(SimpleTool);

	UI_COMMAND(OpenProfilerTab, "Tick Profiler", "Open the Tick Profiler window", EUserInterfaceActionType::Button, FInputChord());
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FProfilingToolsPlusEditorModeCommands::GetCommands()
{
	return FProfilingToolsPlusEditorModeCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
