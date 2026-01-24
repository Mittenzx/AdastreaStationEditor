// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "StationDesignerTypes.h"
#include "ModuleDiscovery.h"

/**
 * Station Viewport Widget - 3D visualization of station design
 */
class SStationViewport : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStationViewport)
		: _StationDesign(nullptr)
		{}
		SLATE_ARGUMENT(FStationDesign*, StationDesign)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	/** Add module to viewport */
	void AddModule(const FModuleInfo& ModuleInfo, const FTransform& Transform);

	/** Remove selected module */
	void RemoveSelectedModule();

	/** Clear all modules */
	void ClearModules();

	/** Get current design */
	const FStationDesign& GetCurrentDesign() const { return CurrentDesign; }

	/** Set current design */
	void SetCurrentDesign(const FStationDesign& Design);

private:
	// Current station design
	FStationDesign CurrentDesign;

	// Selected module index
	int32 SelectedModuleIndex;

	// Generate viewport content
	TSharedRef<SWidget> CreateViewportContent();

	// Handle drag-drop operations
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
};
