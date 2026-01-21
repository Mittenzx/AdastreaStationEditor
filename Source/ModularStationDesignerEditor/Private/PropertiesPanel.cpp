// Copyright Epic Games, Inc. All Rights Reserved.

#include "PropertiesPanel.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "PropertiesPanel"

void SPropertiesPanel::Construct(const FArguments& InArgs)
{
	SelectedModule = nullptr;
	
	ChildSlot
	[
		SNew(SScrollBox)
		.Orientation(Orient_Vertical)
		
		// Station info section
		+ SScrollBox::Slot()
		.Padding(5.0f)
		[
			CreateStationInfo()
		]
		
		+ SScrollBox::Slot()
		.Padding(5.0f, 0.0f)
		[
			SNew(SSeparator)
			.Thickness(2.0f)
		]
		
		// Module properties section
		+ SScrollBox::Slot()
		.Padding(5.0f)
		[
			CreateModuleProperties()
		]
		
		+ SScrollBox::Slot()
		.Padding(5.0f, 0.0f)
		[
			SNew(SSeparator)
			.Thickness(2.0f)
		]
		
		// Statistics section
		+ SScrollBox::Slot()
		.Padding(5.0f)
		[
			CreateStatistics()
		]
		
		+ SScrollBox::Slot()
		.Padding(5.0f, 0.0f)
		[
			SNew(SSeparator)
			.Thickness(2.0f)
		]
		
		// Validation results section
		+ SScrollBox::Slot()
		.Padding(5.0f)
		[
			CreateValidationResults()
		]
	];
}

void SPropertiesPanel::SetStationDesign(const FStationDesign& Design)
{
	CurrentDesign = Design;
	// Trigger UI refresh
}

void SPropertiesPanel::SetSelectedModule(const FModulePlacement* Module)
{
	SelectedModule = Module;
	// Trigger UI refresh
}

void SPropertiesPanel::ClearSelection()
{
	SelectedModule = nullptr;
	// Trigger UI refresh
}

TSharedRef<SWidget> SPropertiesPanel::CreateStationInfo()
{
	return SNew(SVerticalBox)
		
		// Header
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 5.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("StationInfoHeader", "Station Properties"))
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		]
		
		// Station name label
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("StationNameLabel", "Station Name:"))
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
		]
		
		// Station name input
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f, 0.0f, 5.0f)
		[
			SNew(SEditableTextBox)
			.Text(this, &SPropertiesPanel::GetStationName)
			.OnTextChanged(this, &SPropertiesPanel::OnStationNameChanged)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
		]
		
		// Design version
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]()
			{
				return FText::Format(
					LOCTEXT("DesignVersion", "Version: {0}"),
					FText::FromString(CurrentDesign.DesignVersion)
				);
			})
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			.ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f, 1.0f))
		];
}

TSharedRef<SWidget> SPropertiesPanel::CreateModuleProperties()
{
	return SNew(SVerticalBox)
		
		// Header
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 5.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ModulePropertiesHeader", "Selected Module"))
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		]
		
		// Module info or "No selection" message
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBorder)
			.Padding(5.0f)
			.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.5f))
			[
				SNew(SVerticalBox)
				
				// Module ID
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 2.0f)
				[
					SNew(STextBlock)
					.Text_Lambda([this]()
					{
						if (SelectedModule)
						{
							return FText::Format(
								LOCTEXT("ModuleID", "ID: {0}"),
								FText::FromString(SelectedModule->ModuleID)
							);
						}
						return LOCTEXT("NoModuleSelected", "No module selected");
					})
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
				]
				
				// Component name
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 2.0f)
				[
					SNew(STextBlock)
					.Text_Lambda([this]()
					{
						if (SelectedModule)
						{
							return FText::Format(
								LOCTEXT("ComponentName", "Component: {0}"),
								FText::FromString(SelectedModule->ComponentName)
							);
						}
						return FText::GetEmpty();
					})
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
					.Visibility_Lambda([this]()
					{
						return SelectedModule ? EVisibility::Visible : EVisibility::Collapsed;
					})
				]
				
				// Transform info
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 2.0f)
				[
					SNew(STextBlock)
					.Text(this, &SPropertiesPanel::GetSelectedModuleTransform)
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
					.ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f, 1.0f))
					.Visibility_Lambda([this]()
					{
						return SelectedModule ? EVisibility::Visible : EVisibility::Collapsed;
					})
				]
				
				// Connections count
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 2.0f)
				[
					SNew(STextBlock)
					.Text_Lambda([this]()
					{
						if (SelectedModule)
						{
							return FText::Format(
								LOCTEXT("ConnectionCount", "Connections: {0}"),
								FText::AsNumber(SelectedModule->ConnectedModuleIDs.Num())
							);
						}
						return FText::GetEmpty();
					})
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
					.Visibility_Lambda([this]()
					{
						return SelectedModule ? EVisibility::Visible : EVisibility::Collapsed;
					})
				]
			]
		];
}

TSharedRef<SWidget> SPropertiesPanel::CreateStatistics()
{
	return SNew(SVerticalBox)
		
		// Header
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 5.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("StatisticsHeader", "Station Statistics"))
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		]
		
		// Module count
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text(this, &SPropertiesPanel::GetModuleCount)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
		]
		
		// Power balance
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text(this, &SPropertiesPanel::GetPowerBalance)
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			.ColorAndOpacity_Lambda([this]()
			{
				// TODO: Calculate actual power balance
				// Green if positive, red if negative, yellow if zero
				return FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);
			})
		]
		
		// Connection count
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]()
			{
				// Count total connections
				int32 ConnectionCount = 0;
				for (const FModulePlacement& Module : CurrentDesign.Modules)
				{
					ConnectionCount += Module.ConnectedModuleIDs.Num();
				}
				// Divide by 2 since each connection is counted twice
				ConnectionCount /= 2;
				
				return FText::Format(
					LOCTEXT("TotalConnections", "Connections: {0}"),
					FText::AsNumber(ConnectionCount)
				);
			})
			.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
		];
}

TSharedRef<SWidget> SPropertiesPanel::CreateValidationResults()
{
	return SNew(SVerticalBox)
		
		// Header
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 5.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ValidationHeader", "Validation"))
			.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
		]
		
		// Placeholder validation message
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 2.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ValidationPlaceholder", "Run validation to check for issues"))
			.Font(FCoreStyle::GetDefaultFontStyle("Italic", 9))
			.ColorAndOpacity(FLinearColor(0.6f, 0.6f, 0.6f, 1.0f))
		];
}

FText SPropertiesPanel::GetStationName() const
{
	return FText::FromString(CurrentDesign.StationName);
}

FText SPropertiesPanel::GetModuleCount() const
{
	return FText::Format(
		LOCTEXT("ModuleCountDisplay", "Modules: {0}"),
		FText::AsNumber(CurrentDesign.Modules.Num())
	);
}

FText SPropertiesPanel::GetPowerBalance() const
{
	// TODO: Calculate actual power balance from modules
	return LOCTEXT("PowerBalancePlaceholder", "Power Balance: N/A");
}

FText SPropertiesPanel::GetSelectedModuleName() const
{
	if (SelectedModule)
	{
		return FText::FromString(SelectedModule->ComponentName);
	}
	return LOCTEXT("NoSelection", "No Selection");
}

FText SPropertiesPanel::GetSelectedModuleTransform() const
{
	if (SelectedModule)
	{
		FVector Location = SelectedModule->Transform.GetLocation();
		return FText::Format(
			LOCTEXT("TransformDisplay", "Position: X={0}, Y={1}, Z={2}"),
			FText::AsNumber(FMath::RoundToInt(Location.X)),
			FText::AsNumber(FMath::RoundToInt(Location.Y)),
			FText::AsNumber(FMath::RoundToInt(Location.Z))
		);
	}
	return FText::GetEmpty();
}

void SPropertiesPanel::OnStationNameChanged(const FText& NewName)
{
	// TODO: This modifies a local copy of CurrentDesign. Changes should be propagated
	// back to SStationDesignerWindow through a callback mechanism or shared pointer.
	CurrentDesign.StationName = NewName.ToString();
}

void SPropertiesPanel::OnModuleTransformChanged(const FText& NewValue, ETextCommit::Type CommitType)
{
	// TODO: Parse and update module transform
	// This would require more complex UI with separate X, Y, Z inputs
}

#undef LOCTEXT_NAMESPACE
