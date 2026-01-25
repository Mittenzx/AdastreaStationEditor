// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularStationDesignerEditor.h"
#include "StationDesignerWindow.h"
#include "Modules/ModuleManager.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "Widgets/Docking/SDockTab.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "FModularStationDesignerEditorModule"

static const FName StationDesignerTabName("StationDesigner");

void FModularStationDesignerEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory
	UE_LOG(LogTemp, Log, TEXT("ModularStationDesignerEditor: Module Started"));

	// Register tab spawner
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		StationDesignerTabName,
		FOnSpawnTab::CreateRaw(this, &FModularStationDesignerEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("StationDesignerTabTitle", "Station Designer"))
		.SetMenuType(ETabSpawnerMenuType::Enabled)
		.SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Viewports"))
		.SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory());

	// Register menus
	RegisterMenus();
}

void FModularStationDesignerEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module
	UE_LOG(LogTemp, Log, TEXT("ModularStationDesignerEditor: Module Shutdown"));

	// Unregister ToolMenus startup callback and menu entries owned by this module
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	// Unregister tab spawner
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(StationDesignerTabName);
}

void FModularStationDesignerEditorModule::RegisterMenus()
{
	// Register menu extensions
	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(
			this, &FModularStationDesignerEditorModule::RegisterMenuExtensions));
}

void FModularStationDesignerEditorModule::RegisterMenuExtensions()
{
	// Register the Tools menu entry
	UToolMenu* ToolsMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");
	if (ToolsMenu)
	{
		FToolMenuSection& Section = ToolsMenu->FindOrAddSection("Adastrea");
		Section.AddMenuEntry(
			"OpenStationDesigner",
			LOCTEXT("OpenStationDesigner", "Station Designer"),
			LOCTEXT("OpenStationDesignerTooltip", "Open the Modular Station Designer tool"),
			FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Viewports"),
			FUIAction(FExecuteAction::CreateRaw(this, &FModularStationDesignerEditorModule::OnStationBuilderButtonClicked))
		);
	}
}

void FModularStationDesignerEditorModule::OnStationBuilderButtonClicked()
{
	// Open the Station Designer tab
	FGlobalTabmanager::Get()->TryInvokeTab(StationDesignerTabName);
}

TSharedRef<SDockTab> FModularStationDesignerEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& /*SpawnTabArgs*/)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SStationDesignerWindow)
		];
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModularStationDesignerEditorModule, ModularStationDesignerEditor)
