// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProfilingToolsPlusEditorModeToolkit.h"
#include "ProfilingToolsPlusEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "ProfilingToolsPlusEditorModeToolkit"

FProfilingToolsPlusEditorModeToolkit::FProfilingToolsPlusEditorModeToolkit()
{
}

void FProfilingToolsPlusEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FProfilingToolsPlusEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FProfilingToolsPlusEditorModeToolkit::GetToolkitFName() const
{
	return FName("ProfilingToolsPlusEditorMode");
}

FText FProfilingToolsPlusEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "ProfilingToolsPlusEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
