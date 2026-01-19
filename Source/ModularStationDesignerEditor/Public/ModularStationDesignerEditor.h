// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FToolBarBuilder;
class FMenuBuilder;

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
	void RegisterMenus();
	void OnStationBuilderButtonClicked();

	TSharedPtr<class FUICommandList> PluginCommands;
};
