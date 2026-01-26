// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SEditorViewport.h"
#include "StationDesignerTypes.h"
#include "ModuleDiscovery.h"

class FStationViewportClient;
class FPreviewScene;

/**
 * Station Viewport Widget - 3D visualization of station design
 * Provides real-time 3D rendering of modules, connections, and visual overlays
 */
class SStationViewport : public SEditorViewport
{
public:
	SLATE_BEGIN_ARGS(SStationViewport)
		: _StationDesign(nullptr)
		{}
		SLATE_ARGUMENT(FStationDesign*, StationDesign)
	SLATE_END_ARGS()

	/** Constructor/Destructor */
	SStationViewport();
	virtual ~SStationViewport();

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

	/** Refresh viewport rendering */
	void RefreshViewport();

protected:
	// SEditorViewport interface
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;

	// Handle drag-drop operations
	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

private:
	// Pointer to external station design (if provided) or nullptr
	FStationDesign* ExternalDesign;
	
	// Internal station design (used when no external design provided)
	FStationDesign InternalDesign;

	// Selected module index
	int32 SelectedModuleIndex;
	
	// Get the active design (external if available, otherwise internal)
	FStationDesign& GetActiveDesign() { return ExternalDesign ? *ExternalDesign : InternalDesign; }

	// Viewport client for 3D rendering
	TSharedPtr<FStationViewportClient> ViewportClient;

	// Preview scene for rendering
	TSharedPtr<FPreviewScene> PreviewScene;
};
