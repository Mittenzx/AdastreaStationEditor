// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationDesignerWindow.h"
#include "ModulePalette.h"
#include "PropertiesPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "DesktopPlatformModule.h"

#define LOCTEXT_NAMESPACE "StationDesignerWindow"

void SStationDesignerWindow::Construct(const FArguments& InArgs)
{
	// Initialize design
	CurrentDesign = FStationDesign();

	ChildSlot
	[
		SNew(SVerticalBox)
		
		// Toolbar
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			CreateToolbar()
		]

		// Main content area with splitter
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			CreateMainContent()
		]

		// Status bar
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			CreateStatusBar()
		]
	];
}

TSharedRef<SWidget> SStationDesignerWindow::CreateToolbar()
{
	return SNew(SHorizontalBox)
		
		// New button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("NewButton", "New"))
			.ToolTipText(LOCTEXT("NewButtonTooltip", "Create a new station design"))
			.OnClicked(this, &SStationDesignerWindow::OnNewStation)
		]

		// Load button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("LoadButton", "Load"))
			.ToolTipText(LOCTEXT("LoadButtonTooltip", "Load a station design from file"))
			.OnClicked(this, &SStationDesignerWindow::OnLoadStation)
		]

		// Save button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("SaveButton", "Save"))
			.ToolTipText(LOCTEXT("SaveButtonTooltip", "Save the current station design"))
			.OnClicked(this, &SStationDesignerWindow::OnSaveStation)
		]

		// Export button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("ExportButton", "Export"))
			.ToolTipText(LOCTEXT("ExportButtonTooltip", "Export station to Blueprint"))
			.OnClicked(this, &SStationDesignerWindow::OnExportStation)
		]

		// Validate button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("ValidateButton", "Validate"))
			.ToolTipText(LOCTEXT("ValidateButtonTooltip", "Check station design for issues"))
			.OnClicked(this, &SStationDesignerWindow::OnValidateStation)
		]
		
		// Spacer
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SSpacer)
		]
		
		// Refresh modules button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("RefreshButton", "Refresh Modules"))
			.ToolTipText(LOCTEXT("RefreshButtonTooltip", "Rescan for available modules"))
			.OnClicked(this, &SStationDesignerWindow::OnRefreshModules)
		];
}

TSharedRef<SWidget> SStationDesignerWindow::CreateMainContent()
{
	// Three-panel layout: Module Palette | 3D Viewport | Properties Panel
	return SNew(SSplitter)
		.Orientation(Orient_Horizontal)
		
		// Left panel: Module Palette
		+ SSplitter::Slot()
		.Value(0.2f)
		[
			CreateModulePalettePanel()
		]
		
		// Center panel: 3D Viewport
		+ SSplitter::Slot()
		.Value(0.6f)
		[
			CreateViewportPanel()
		]
		
		// Right panel: Properties
		+ SSplitter::Slot()
		.Value(0.2f)
		[
			CreatePropertiesPanel()
		];
}

TSharedRef<SWidget> SStationDesignerWindow::CreateStatusBar()
{
	return SNew(SBorder)
		.Padding(5.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]()
			{
				int32 ModuleCount = CurrentDesign.Modules.Num();
				// TODO: Calculate actual power balance
				return FText::Format(
					LOCTEXT("StatusBar", "Ready | Modules: {0} | Power Balance: 0 MW"),
					FText::AsNumber(ModuleCount)
				);
			})
		];
}

TSharedRef<SWidget> SStationDesignerWindow::CreateModulePalettePanel()
{
	return SNew(SBorder)
		.Padding(5.0f)
		.BorderBackgroundColor(FLinearColor(0.02f, 0.02f, 0.02f, 1.0f))
		[
			SNew(SVerticalBox)
			
			// Panel header
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ModulePaletteHeader", "Module Palette"))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]
			
			// Module palette widget
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(ModulePalette, SModulePalette)
			]
		];
}

TSharedRef<SWidget> SStationDesignerWindow::CreateViewportPanel()
{
	return SNew(SBorder)
		.Padding(10.0f)
		.BorderBackgroundColor(FLinearColor(0.05f, 0.05f, 0.05f, 1.0f))
		[
			SNew(SVerticalBox)
			
			// Viewport header
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ViewportHeader", "3D Viewport"))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]
			
			// Viewport placeholder
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.15f, 1.0f))
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("ViewportPlaceholder", "3D Viewport\n\n[Viewport integration in progress]\n\nThis will show:\n• Module placement visualization\n• Connection points\n• Drag-and-drop preview\n• Camera controls"))
					.Justification(ETextJustify::Center)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
					.ColorAndOpacity(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f))
				]
			]
		];
}

TSharedRef<SWidget> SStationDesignerWindow::CreatePropertiesPanel()
{
	return SNew(SBorder)
		.Padding(5.0f)
		.BorderBackgroundColor(FLinearColor(0.02f, 0.02f, 0.02f, 1.0f))
		[
			SNew(SVerticalBox)
			
			// Panel header
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("PropertiesPanelHeader", "Properties"))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
			]
			
			// Properties panel widget
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(PropertiesPanel, SPropertiesPanel)
			]
		];
}

FReply SStationDesignerWindow::OnNewStation()
{
	CurrentDesign = FStationDesign();
	UpdateUI();
	UE_LOG(LogTemp, Log, TEXT("New station created"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnLoadStation()
{
	// Open file dialog
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TArray<FString> OutFiles;
		const FString DefaultPath = FPaths::ProjectSavedDir() / TEXT("StationDesigns");
		
		if (DesktopPlatform->OpenFileDialog(
			nullptr,
			TEXT("Load Station Design"),
			DefaultPath,
			TEXT(""),
			TEXT("Station Design Files (*.station)|*.station"),
			EFileDialogFlags::None,
			OutFiles))
		{
			if (OutFiles.Num() > 0)
			{
				LoadStationFromFile(OutFiles[0]);
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Load station clicked"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnSaveStation()
{
	// Open save dialog
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TArray<FString> OutFiles;
		const FString DefaultPath = FPaths::ProjectSavedDir() / TEXT("StationDesigns");
		const FString DefaultFile = CurrentDesign.StationName + TEXT(".station");
		
		if (DesktopPlatform->SaveFileDialog(
			nullptr,
			TEXT("Save Station Design"),
			DefaultPath,
			DefaultFile,
			TEXT("Station Design Files (*.station)|*.station"),
			EFileDialogFlags::None,
			OutFiles))
		{
			if (OutFiles.Num() > 0)
			{
				SaveStationToFile(OutFiles[0]);
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Save station clicked"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnExportStation()
{
	// TODO: Implement Blueprint export using FStationExporter
	UE_LOG(LogTemp, Log, TEXT("Export station clicked - Blueprint export not yet implemented"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnValidateStation()
{
	// TODO: Implement validation using FStationValidator
	UE_LOG(LogTemp, Log, TEXT("Validate station clicked - Validation not yet implemented"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnRefreshModules()
{
	if (ModulePalette.IsValid())
	{
		ModulePalette->RefreshModuleList();
		UE_LOG(LogTemp, Log, TEXT("Module list refreshed"));
	}
	return FReply::Handled();
}

void SStationDesignerWindow::UpdateUI()
{
	if (PropertiesPanel.IsValid())
	{
		PropertiesPanel->SetStationDesign(CurrentDesign);
	}
}

void SStationDesignerWindow::SaveStationToFile(const FString& FilePath)
{
	FString JsonString;
	if (FJsonObjectConverter::UStructToJsonObjectString(CurrentDesign, JsonString))
	{
		if (FFileHelper::SaveStringToFile(JsonString, *FilePath))
		{
			UE_LOG(LogTemp, Log, TEXT("Station saved to: %s"), *FilePath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save station to: %s"), *FilePath);
		}
	}
}

void SStationDesignerWindow::LoadStationFromFile(const FString& FilePath)
{
	FString JsonString;
	if (FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &CurrentDesign, 0, 0))
		{
			UpdateUI();
			UE_LOG(LogTemp, Log, TEXT("Station loaded from: %s"), *FilePath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse station file: %s"), *FilePath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load station file: %s"), *FilePath);
	}
}

#undef LOCTEXT_NAMESPACE
