// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProfilingToolsPlusModule.h"

#include "LevelEditor.h"
#include "ProfilingToolsPlusEditorModeCommands.h"
#include "STickProfilingWindow.h"

#define LOCTEXT_NAMESPACE "ProfilingToolsPlusModule"

void FProfilingToolsPlusModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FProfilingToolsPlusEditorModeCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FProfilingToolsPlusEditorModeCommands::Get().OpenProfilerTab,
		FExecuteAction::CreateRaw(this, &FProfilingToolsPlusModule::OnOpenProfilerTabClicked)
	);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender);
	MenuExtender->AddMenuExtension(
		"WindowLayout",
		EExtensionHook::After,
		PluginCommands,
		FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& MenuBuilder)
		{
			MenuBuilder.AddMenuEntry(FProfilingToolsPlusEditorModeCommands::Get().OpenProfilerTab);
		})
	);
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	TSharedRef<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Settings",
		EExtensionHook::After,
		PluginCommands,
		FToolBarExtensionDelegate::CreateLambda([](FToolBarBuilder& ToolbarBuilder)
		{
			ToolbarBuilder.AddToolBarButton(FProfilingToolsPlusEditorModeCommands::Get().OpenProfilerTab);
		})
	);
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	FEditorDelegates::PostPIEStarted.AddRaw(this, &FProfilingToolsPlusModule::OnBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &FProfilingToolsPlusModule::OnEndPIE);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
	"TickProfilerTab",
	FOnSpawnTab::CreateRaw(this, &FProfilingToolsPlusModule::OnSpawnTickProfilingWindow)).SetDisplayName(LOCTEXT("TickProfilerTabTitle", "Tick Profiler")).SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FProfilingToolsPlusModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("TickProfilerTab");
	FEditorDelegates::PostPIEStarted.RemoveAll(this);
	FEditorDelegates::EndPIE.RemoveAll(this);
	FProfilingToolsPlusEditorModeCommands::Unregister();
}

TSharedRef<SDockTab> FProfilingToolsPlusModule::OnSpawnTickProfilingWindow(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(STickProfilingWindow)
		];
}

void FProfilingToolsPlusModule::OnOpenProfilerTabClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FName("TickProfilerTab"));
}

void FProfilingToolsPlusModule::OnBeginPIE(const bool bIsSimulating)
{
	UE_LOG(LogTemp, Warning, TEXT("Begin PIE, synchronising blacklists"));
	UWorld* EditorWorld = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
	UWorld* PIEWorld = GEditor ? GEditor->PlayWorld : nullptr;

	if (EditorWorld && PIEWorld)
	{
		UE_LOG(LogTemp, Warning, TEXT("Contexts found, Searching for subsystems"));
		UCoreProfilingSubsystem* EditorSubsystem = EditorWorld->GetSubsystem<UCoreProfilingSubsystem>();
		UCoreProfilingSubsystem* PIESubsystem = PIEWorld->GetSubsystem<UCoreProfilingSubsystem>();

		if (EditorSubsystem && PIESubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Subsystems found, synchronising blacklists"));
			// Copy blacklist from editor to PIE
			EditorSubsystem->CopySubsystemsSettingsTo(PIESubsystem);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Subsystems not found, Editor : %p, PIE: %p"), EditorSubsystem, PIESubsystem);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Contexts not found, Editor : %p, PIE: %p"), EditorWorld, PIEWorld);
	}
	RefreshTickProfilingWindow();
}

void FProfilingToolsPlusModule::OnEndPIE(const bool bIsSimulating)
{
	UE_LOG(LogTemp, Warning, TEXT("End PIE, synchronising blacklists"));
	UWorld* EditorWorld = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
	UWorld* PIEWorld = GEditor ? GEditor->PlayWorld : nullptr;

	if (EditorWorld && PIEWorld)
	{
		UE_LOG(LogTemp, Warning, TEXT("Contexts found, Searching for subsystems"));
		UCoreProfilingSubsystem* EditorSubsystem = EditorWorld->GetSubsystem<UCoreProfilingSubsystem>();
		UCoreProfilingSubsystem* PIESubsystem = PIEWorld->GetSubsystem<UCoreProfilingSubsystem>();

		if (EditorSubsystem && PIESubsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Subsystems found, synchronising blacklists"));
			// Copy blacklist from PIE back to editor
			PIESubsystem->CopySubsystemsSettingsTo(EditorSubsystem);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Subsystems not found, Editor : %p, PIE: %p"), EditorSubsystem, PIESubsystem);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Contexts not found, Editor : %p, PIE: %p"), EditorWorld, PIEWorld);
	}
	RefreshTickProfilingWindow();
}

void FProfilingToolsPlusModule::RefreshTickProfilingWindow()
{
	UE_LOG(LogTemp, Warning, TEXT("Refreshing Tick Profiling Window"));
	TSharedPtr<SDockTab> TickProfilerTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("TickProfilerTab"));
	if (TickProfilerTab.IsValid())
	{
		TSharedPtr<STickProfilingWindow> TickProfilingWindow = StaticCastSharedRef<STickProfilingWindow>(TickProfilerTab->GetContent());
		if (TickProfilingWindow.IsValid())
		{
			TickProfilingWindow->RebuildBlacklistRows();
		}
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProfilingToolsPlusModule, ProfilingToolsPlusEditorMode)