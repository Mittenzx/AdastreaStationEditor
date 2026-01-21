// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "StationDesignerTypes.h"

/**
 * Properties Panel Widget
 * Displays and allows editing of selected module properties
 * Shows station statistics and validation results
 */
class SPropertiesPanel : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPropertiesPanel) {}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	/** Set the current station design */
	void SetStationDesign(const FStationDesign& Design);
	
	/** Set the currently selected module */
	void SetSelectedModule(const FModulePlacement* Module);
	
	/** Clear the selection */
	void ClearSelection();

private:
	// Current data
	FStationDesign CurrentDesign;
	const FModulePlacement* SelectedModule;
	
	// UI Generation
	TSharedRef<SWidget> CreateStationInfo();
	TSharedRef<SWidget> CreateModuleProperties();
	TSharedRef<SWidget> CreateStatistics();
	TSharedRef<SWidget> CreateValidationResults();
	
	// Cached widgets for updates
	TSharedPtr<SWidget> StationInfoWidget;
	TSharedPtr<SWidget> ModulePropertiesWidget;
	TSharedPtr<SWidget> StatisticsWidget;
	TSharedPtr<SWidget> ValidationResultsWidget;
	
	// Helper methods
	FText GetStationName() const;
	FText GetModuleCount() const;
	FText GetPowerBalance() const;
	FText GetSelectedModuleName() const;
	FText GetSelectedModuleTransform() const;
	
	// Event handlers
	void OnStationNameChanged(const FText& NewName);
	void OnModuleTransformChanged(const FText& NewValue, ETextCommit::Type CommitType);
};
