// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "ModuleDiscovery.h"

class SSearchBox;
template <typename ItemType> class SListView;

/**
 * Module Palette Widget - displays available modules for placement
 */
class SModulePalette : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SModulePalette) {}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

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
};
