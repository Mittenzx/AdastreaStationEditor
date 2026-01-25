// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FToolBarBuilder;
class FMenuBuilder;
class SDockTab;
class FSpawnTabArgs;

/**
 * Modular Station Designer Editor Module
 * Provides UI and tools for designing space stations
 */
class FModularStationDesignerEditorModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Register menus and extensions */
	void RegisterMenus();
	
	/** Register menu extensions callback */
	void RegisterMenuExtensions();
	
	/** Handle station builder menu button click */
	void OnStationBuilderButtonClicked();
	
	/** Spawn the plugin tab */
	TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);

	TSharedPtr<class FUICommandList> PluginCommands;
};
