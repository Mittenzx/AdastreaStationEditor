// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "ModuleDiscovery.h"

<<<<<<< HEAD
/**
 * Module Palette Widget
 * Displays available station modules in a searchable, filterable grid
 * Provides drag-and-drop functionality for module placement
=======
class SSearchBox;
class SListView;

/**
 * Module Palette Widget - displays available modules for placement
>>>>>>> origin/main
 */
class SModulePalette : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModulePalette) {}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

<<<<<<< HEAD
	/** Refresh the module list from the asset registry */
	void RefreshModuleList();
	
	/** Set the active filter for module groups */
	void SetGroupFilter(EStationModuleGroup Filter);
	
	/** Get the currently selected module info */
	TSharedPtr<FModuleInfo> GetSelectedModule() const;

private:
	// Available modules
	TArray<TSharedPtr<FModuleInfo>> AvailableModules;
	TArray<TSharedPtr<FModuleInfo>> FilteredModules;
	
	// Current filter settings
	EStationModuleGroup CurrentGroupFilter;
	FString SearchText;
	
	// Currently selected module
	TSharedPtr<FModuleInfo> SelectedModule;
	
	// UI Generation
	TSharedRef<SWidget> CreateSearchBar();
	TSharedRef<SWidget> CreateFilterBar();
	TSharedRef<SWidget> CreateModuleGrid();
	TSharedRef<SWidget> CreateModuleTile(TSharedPtr<FModuleInfo> Module);
	
	// Event Handlers
	void OnSearchTextChanged(const FText& Text);
	void OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitType);
	void OnGroupFilterChanged(EStationModuleGroup NewFilter);
	FReply OnModuleClicked(TSharedPtr<FModuleInfo> Module);
	FReply OnModuleDragDetected(const FGeometry& Geometry, const FPointerEvent& PointerEvent, TSharedPtr<FModuleInfo> Module);
	
	// Filtering
	void ApplyFilters();
	bool PassesFilter(TSharedPtr<FModuleInfo> Module) const;
	
	// Module grid widget (cached for updates)
	TSharedPtr<SWidget> ModuleGridWidget;
=======
	/** Refresh module list */
	void RefreshModuleList();

private:
	// Module list data
	TArray<TSharedPtr<FModuleInfo>> AllModules;
	TArray<TSharedPtr<FModuleInfo>> FilteredModules;
	
	// Current filter
	EStationModuleGroup CurrentGroupFilter;
	FString CurrentSearchText;

	// Slate widgets
	TSharedPtr<SListView<TSharedPtr<FModuleInfo>>> ModuleListView;
	TSharedPtr<SSearchBox> SearchBox;

	// Generate row widget for module list
	TSharedRef<ITableRow> OnGenerateModuleRow(
		TSharedPtr<FModuleInfo> Item,
		const TSharedRef<STableViewBase>& OwnerTable);

	// Handle drag-drop from list
	FReply OnModuleDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, TSharedPtr<FModuleInfo> ModuleInfo);

	// Search text changed
	void OnSearchTextChanged(const FText& InFilterText);

	// Group filter changed
	void OnGroupFilterChanged(EStationModuleGroup NewFilter);

	// Apply current filters
	void ApplyFilters();

	// Create group filter buttons
	TSharedRef<SWidget> CreateGroupFilters();
>>>>>>> origin/main
};
