// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationDesignerWindow.h"
#include "ModulePalette.h"
#include "StationViewport.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"

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

		// Main content area
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
			.OnClicked(this, &SStationDesignerWindow::OnNewStation)
		]

		// Load button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("LoadButton", "Load"))
			.OnClicked(this, &SStationDesignerWindow::OnLoadStation)
		]

		// Save button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("SaveButton", "Save"))
			.OnClicked(this, &SStationDesignerWindow::OnSaveStation)
		]

		// Export button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("ExportButton", "Export"))
			.OnClicked(this, &SStationDesignerWindow::OnExportStation)
		]

		// Validate button
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("ValidateButton", "Validate"))
			.OnClicked(this, &SStationDesignerWindow::OnValidateStation)
		];
}

TSharedRef<SWidget> SStationDesignerWindow::CreateMainContent()
{
	return SNew(SBorder)
		.Padding(4.0f)
		[
			SNew(SSplitter)
			.Orientation(Orient_Horizontal)

			// Left panel - Module Palette
			+ SSplitter::Slot()
			.Value(0.25f)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				.Padding(4.0f)
				[
					SNew(SVerticalBox)
					
					// Palette header
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(0.0f, 0.0f, 0.0f, 4.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("ModulePaletteTitle", "Module Palette"))
						.Font(FEditorStyle::GetFontStyle("BoldFont"))
					]

					// Palette content
					+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					[
						SAssignNew(ModulePalette, SModulePalette)
					]
				]
			]

			// Center panel - 3D Viewport
			+ SSplitter::Slot()
			.Value(0.55f)
			[
				SAssignNew(StationViewport, SStationViewport)
			]

			// Right panel - Properties (placeholder)
			+ SSplitter::Slot()
			.Value(0.20f)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
				.Padding(10.0f)
				[
					SNew(SVerticalBox)
					
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PropertiesTitle", "Properties"))
						.Font(FEditorStyle::GetFontStyle("BoldFont"))
					]

					+ SVerticalBox::Slot()
					.FillHeight(1.0f)
					.Padding(0.0f, 10.0f, 0.0f, 0.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("PropertiesPlaceholder", "Select a module to view properties"))
						.AutoWrapText(true)
					]
				]
			]
		];
}

TSharedRef<SWidget> SStationDesignerWindow::CreateStatusBar()
{
	return SNew(SBorder)
		.Padding(5.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]() {
				if (StationViewport.IsValid())
				{
					const FStationDesign& Design = StationViewport->GetCurrentDesign();
					return FText::FromString(FString::Printf(
						TEXT("Ready | Modules: %d | Station: %s"),
						Design.Modules.Num(),
						*Design.StationName));
				}
				return LOCTEXT("StatusBarDefault", "Ready");
			})
		];
}

FReply SStationDesignerWindow::OnNewStation()
{
	CurrentDesign = FStationDesign();
	if (StationViewport.IsValid())
	{
		StationViewport->SetCurrentDesign(CurrentDesign);
	}
	UE_LOG(LogTemp, Log, TEXT("New station created"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnLoadStation()
{
	UE_LOG(LogTemp, Log, TEXT("Load station clicked"));
	// TODO: Implement load functionality
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnSaveStation()
{
	if (StationViewport.IsValid())
	{
		CurrentDesign = StationViewport->GetCurrentDesign();
		UE_LOG(LogTemp, Log, TEXT("Saving station: %s with %d modules"),
			*CurrentDesign.StationName, CurrentDesign.Modules.Num());
	}
	// TODO: Implement save functionality
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnExportStation()
{
	if (StationViewport.IsValid())
	{
		CurrentDesign = StationViewport->GetCurrentDesign();
		UE_LOG(LogTemp, Log, TEXT("Exporting station: %s"), *CurrentDesign.StationName);
	}
	// TODO: Implement export functionality
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnValidateStation()
{
	if (StationViewport.IsValid())
	{
		CurrentDesign = StationViewport->GetCurrentDesign();
		UE_LOG(LogTemp, Log, TEXT("Validating station: %s"), *CurrentDesign.StationName);
	}
	// TODO: Implement validation functionality
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
