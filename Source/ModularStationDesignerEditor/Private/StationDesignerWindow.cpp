// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationDesignerWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
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
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("MainContentPlaceholder", "Station Designer - Main Content Area\n\nThis would contain:\n- Module Palette (left panel)\n- 3D Viewport (center)\n- Properties Panel (right)"))
			.Justification(ETextJustify::Center)
		];
}

TSharedRef<SWidget> SStationDesignerWindow::CreateStatusBar()
{
	return SNew(SBorder)
		.Padding(5.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("StatusBar", "Ready | Modules: 0 | Power Balance: 0 MW"))
		];
}

FReply SStationDesignerWindow::OnNewStation()
{
	CurrentDesign = FStationDesign();
	UE_LOG(LogTemp, Log, TEXT("New station created"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnLoadStation()
{
	UE_LOG(LogTemp, Log, TEXT("Load station clicked"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnSaveStation()
{
	UE_LOG(LogTemp, Log, TEXT("Save station clicked"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnExportStation()
{
	UE_LOG(LogTemp, Log, TEXT("Export station clicked"));
	return FReply::Handled();
}

FReply SStationDesignerWindow::OnValidateStation()
{
	UE_LOG(LogTemp, Log, TEXT("Validate station clicked"));
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
