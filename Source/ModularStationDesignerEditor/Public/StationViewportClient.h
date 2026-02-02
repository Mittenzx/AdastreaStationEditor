// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"
#include "StationDesignerTypes.h"

class FPreviewScene;
class SStationViewport;
class UStaticMeshComponent;

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

	/** Preview scene reference for spawning components */
	FPreviewScene* PreviewScene;

	/** Map of module IDs to their preview components */
	TMap<FString, UStaticMeshComponent*> PreviewComponents;

	/** Map of module IDs to their blueprint paths (for detecting changes) */
	TMap<FString, FSoftClassPath> ModuleBlueprintPaths;

	/** Update preview components to match current design */
	void UpdatePreviewComponents();

	/** Clear all preview components */
	void ClearPreviewComponents();

	/** Helper function to load mesh from blueprint path */
	UStaticMesh* LoadMeshFromBlueprintPath(const FSoftClassPath& BlueprintPath, TArray<UMaterialInterface*>& OutMaterials);

	/** Draw module representations */
	void DrawModules(const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Draw connection wires between modules */
	void DrawConnectionWires(const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Draw power flow visualization */
	void DrawPowerFlow(const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Draw grid overlay */
	void DrawGrid(const FSceneView* View, FPrimitiveDrawInterface* PDI);

	/** Helper function to draw orientation axes for a module */
	void DrawModuleAxes(FPrimitiveDrawInterface* PDI, const FVector& Location, const FTransform& Transform, float AxisLength);

	/** Animation time for effects */
	float AnimationTime;

	/** Cached number of grid lines (computed once) */
	int32 CachedGridLines;
};
