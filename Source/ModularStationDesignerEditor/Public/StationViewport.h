// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "StationDesignerTypes.h"
#include "ModuleDiscovery.h"

/**
 * Station Viewport Widget - 3D visualization of station design
 * 
 * NOTE: This is currently a PLACEHOLDER implementation showing text-based UI.
 * For full 3D rendering, this needs to be replaced with SEditorViewport and
 * FEditorViewportClient to provide actual 3D scene rendering capabilities.
 * See Unreal Engine's SEditorViewport documentation for implementation details.
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
	const FStationDesign& GetCurrentDesign() const { return ExternalDesign ? *ExternalDesign : InternalDesign; }

	/** Set current design */
	void SetCurrentDesign(const FStationDesign& Design);

private:
	// Pointer to external station design (if provided) or nullptr
	FStationDesign* ExternalDesign;
	
	// Internal station design (used when no external design provided)
	FStationDesign InternalDesign;

	// Selected module index
	int32 SelectedModuleIndex;
	
	// Get the active design (external if available, otherwise internal)
	FStationDesign& GetActiveDesign() { return ExternalDesign ? *ExternalDesign : InternalDesign; }

	// Generate viewport content
	TSharedRef<SWidget> CreateViewportContent();

	// Handle drag-drop operations
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
};
