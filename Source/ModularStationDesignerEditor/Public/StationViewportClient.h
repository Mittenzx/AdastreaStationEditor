// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"
#include "StationDesignerTypes.h"

class FPreviewScene;
class SStationViewport;

/**
 * Viewport client for station designer 3D visualization
 * Handles rendering of modules, connections, and visual overlays
 */
class FStationViewportClient : public FEditorViewportClient
{
public:
	FStationViewportClient(FPreviewScene* InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewportWidget);
	virtual ~FStationViewportClient();

	// FEditorViewportClient interface
	virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas) override;
	virtual void Tick(float DeltaSeconds) override;

	/** Set the station design to visualize */
	void SetStationDesign(FStationDesign* InDesign);

	/** Get current station design */
	const FStationDesign* GetStationDesign() const { return CurrentDesign; }

private:
	/** Pointer to the station design being visualized */
	FStationDesign* CurrentDesign;

	/** Draw module representations */
	void DrawModules(const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Draw connection wires between modules */
	void DrawConnectionWires(const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Draw power flow visualization */
	void DrawPowerFlow(const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Draw grid overlay */
	void DrawGrid(const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Animation time for effects */
	float AnimationTime;
};
