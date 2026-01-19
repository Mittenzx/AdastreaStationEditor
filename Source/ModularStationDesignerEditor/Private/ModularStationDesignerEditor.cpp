// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularStationDesignerEditor.h"
#include "Modules/ModuleManager.h"
#include "LevelEditor.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FModularStationDesignerEditorModule"

void FModularStationDesignerEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory
	UE_LOG(LogTemp, Log, TEXT("ModularStationDesignerEditor: Module Started"));

	// Register menus
	RegisterMenus();
}

void FModularStationDesignerEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module
	UE_LOG(LogTemp, Log, TEXT("ModularStationDesignerEditor: Module Shutdown"));
}

void FModularStationDesignerEditorModule::RegisterMenus()
{
	// Register menu extensions
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
		this, &FModularStationDesignerEditorModule::OnStationBuilderButtonClicked));
}

void FModularStationDesignerEditorModule::OnStationBuilderButtonClicked()
{
	// Placeholder for opening Station Builder window
	UE_LOG(LogTemp, Log, TEXT("Station Builder button clicked"));
	
	// In a full implementation, this would open the Slate UI window
	// TSharedRef<SWindow> Window = SNew(SWindow)
	//     .Title(LOCTEXT("StationBuilderTitle", "Station Builder"))
	//     .ClientSize(FVector2D(1280, 720));
	// 
	// TSharedRef<SStationDesignerWindow> StationDesignerWindow = SNew(SStationDesignerWindow);
	// Window->SetContent(StationDesignerWindow);
	// 
	// FSlateApplication::Get().AddWindow(Window);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModularStationDesignerEditorModule, ModularStationDesignerEditor)
