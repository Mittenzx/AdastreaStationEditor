// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationViewportClient.h"
#include "VisualizationSystem.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "PreviewScene.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Engine/Canvas.h"
#include "SceneManagement.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Blueprint.h"
#include "GameFramework/Actor.h"

FStationViewportClient::FStationViewportClient(FPreviewScene* InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewportWidget)
	: FEditorViewportClient(nullptr, InPreviewScene, InEditorViewportWidget)
	, CurrentDesign(nullptr)
	, PreviewScene(InPreviewScene)
	, AnimationTime(0.0f)
	, CachedGridLines(0)
{
	// Set up viewport for 3D visualization
	SetViewMode(VMI_Lit);
	
	// Configure camera
	SetViewLocation(FVector(0, -1000, 500));
	SetViewRotation(FRotator(-15, 0, 0));
	
	// Enable real-time updates for animations
	SetRealtime(true);
	
	// Draw helper features
	bDrawAxes = true;
	EngineShowFlags.SetGrid(true);
	EngineShowFlags.SetSnap(false);

	// Pre-calculate grid lines (GridSize=10000, GridSpacing=100)
	const float GridSize = 10000.0f;
	const float GridSpacing = 100.0f;
	CachedGridLines = FMath::CeilToInt(GridSize / GridSpacing);
}

FStationViewportClient::~FStationViewportClient()
{
	// Clean up all preview components
	ClearPreviewComponents();
}

void FStationViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);
	
	// Update animation time for visual effects
	AnimationTime += DeltaSeconds;
}

void FStationViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);
	
	if (!CurrentDesign)
	{
		return;
	}

	// Draw visualization elements
	DrawGrid(View, PDI);
	DrawModules(View, PDI);
	DrawConnectionWires(View, PDI);
	
	// Optional: Draw power flow if enabled
	if (FVisualizationSystem::GetSettings().bShowPowerFlow)
	{
		DrawPowerFlow(View, PDI);
	}
}

void FStationViewportClient::DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas)
{
	FEditorViewportClient::DrawCanvas(InViewport, View, Canvas);
	
	if (!CurrentDesign)
	{
		// Draw instructions when no design is loaded
		FCanvasTextItem TextItem(
			FVector2D(10, 10),
			FText::FromString(TEXT("Drag modules from the palette to start building your station")),
			GEngine->GetSmallFont(),
			FLinearColor::White
		);
		TextItem.EnableShadow(FLinearColor::Black);
		Canvas.DrawItem(TextItem);
	}
	else
	{
		// Draw module count
		FString InfoText = FString::Printf(TEXT("Modules: %d"), CurrentDesign->Modules.Num());
		FCanvasTextItem TextItem(
			FVector2D(10, 10),
			FText::FromString(InfoText),
			GEngine->GetMediumFont(),
			FLinearColor::White
		);
		TextItem.EnableShadow(FLinearColor::Black);
		Canvas.DrawItem(TextItem);
	}
}

void FStationViewportClient::SetStationDesign(FStationDesign* InDesign)
{
	CurrentDesign = InDesign;
	
	// Update preview components to match new design
	UpdatePreviewComponents();
}

void FStationViewportClient::DrawModules(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (!CurrentDesign)
	{
		return;
	}

	// Draw each module - either the loaded mesh (via preview components) or a wireframe fallback
	for (const FModulePlacement& Module : CurrentDesign->Modules)
	{
		// Check if we have a preview component with a mesh for this module
		UStaticMeshComponent* Component = PreviewComponents.FindRef(Module.ModuleID);
		bool bHasMesh = Component && Component->GetStaticMesh();
		
		if (!bHasMesh)
		{
			// Draw wireframe fallback if mesh couldn't be loaded
			FVector Location = Module.Transform.GetLocation();
			
			// Default module size (can be customized later based on module type)
			FVector BoxExtent(100.0f, 100.0f, 50.0f);
			
			// Get color based on module name (simplified)
			FLinearColor ModuleColor = FLinearColor::White;
			FString ModuleName = Module.ModuleBlueprintPath.GetAssetName();
			
			if (ModuleName.Contains(TEXT("Docking")))
			{
				ModuleColor = FVisualizationSystem::GetColorForModuleGroup(EStationModuleGroup::Docking);
			}
			else if (ModuleName.Contains(TEXT("Power")) || ModuleName.Contains(TEXT("Reactor")))
			{
				ModuleColor = FVisualizationSystem::GetColorForModuleGroup(EStationModuleGroup::Power);
			}
			else if (ModuleName.Contains(TEXT("Storage")) || ModuleName.Contains(TEXT("Cargo")))
			{
				ModuleColor = FVisualizationSystem::GetColorForModuleGroup(EStationModuleGroup::Storage);
			}
			else if (ModuleName.Contains(TEXT("Habitat")))
			{
				ModuleColor = FVisualizationSystem::GetColorForModuleGroup(EStationModuleGroup::Habitation);
			}
			
			// Draw the module as a wireframe box
			FBox Box(Location - BoxExtent, Location + BoxExtent);
			DrawWireBox(PDI, Box, ModuleColor, SDPG_World);
		}
		
		// Always draw orientation indicator
		DrawModuleAxes(PDI, Module.Transform.GetLocation(), Module.Transform, 75.0f);
	}
}

void FStationViewportClient::DrawModuleAxes(FPrimitiveDrawInterface* PDI, const FVector& Location, const FTransform& Transform, float AxisLength)
{
	// Calculate axis directions
	FVector XAxis = Transform.TransformVector(FVector::ForwardVector) * AxisLength;
	FVector YAxis = Transform.TransformVector(FVector::RightVector) * AxisLength;
	FVector ZAxis = Transform.TransformVector(FVector::UpVector) * AxisLength;
	
	// Draw orientation axes
	PDI->DrawLine(Location, Location + XAxis, FLinearColor::Red, SDPG_World, 2.0f);
	PDI->DrawLine(Location, Location + YAxis, FLinearColor::Green, SDPG_World, 2.0f);
	PDI->DrawLine(Location, Location + ZAxis, FLinearColor::Blue, SDPG_World, 2.0f);
}

void FStationViewportClient::DrawConnectionWires(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (!CurrentDesign)
	{
		return;
	}

	// Use visualization system to generate connection wires
	TArray<FVisualizationSystem::FConnectionWire> Wires = FVisualizationSystem::GenerateConnectionWires(*CurrentDesign);
	
	for (const auto& Wire : Wires)
	{
		FLinearColor WireColor = Wire.bIsValid ? FLinearColor::Blue : FLinearColor::Red;
		float Thickness = FVisualizationSystem::GetSettings().WireThickness;
		
		PDI->DrawLine(Wire.StartPoint, Wire.EndPoint, WireColor, SDPG_World, Thickness);
	}
}

void FStationViewportClient::DrawPowerFlow(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (!CurrentDesign)
	{
		return;
	}

	// Use visualization system to generate power flow lines
	TArray<FVisualizationSystem::FPowerFlowVisualization> PowerFlows = FVisualizationSystem::GeneratePowerFlowLines(*CurrentDesign);
	
	for (const auto& Flow : PowerFlows)
	{
		// Animate power flow with moving particles effect
		float AnimSpeed = FVisualizationSystem::GetSettings().PowerFlowAnimationSpeed;
		float Progress = FMath::Fmod(AnimationTime * AnimSpeed, 1.0f);
		
		FVector FlowPosition = FMath::Lerp(Flow.StartPoint, Flow.EndPoint, Progress);
		
		// Draw the connection line
		PDI->DrawLine(Flow.StartPoint, Flow.EndPoint, Flow.Color * 0.5f, SDPG_World, 1.0f);
		
		// Draw animated particle
		float ParticleSize = 10.0f;
		DrawWireSphere(PDI, FlowPosition, Flow.Color, ParticleSize, 8, SDPG_World);
	}
}

void FStationViewportClient::DrawGrid(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	// Draw a simple reference grid at Z=0
	const float GridSize = 10000.0f;
	const float GridSpacing = 100.0f;
	const FLinearColor GridColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.5f);
	
	// Use cached grid lines value (computed once in constructor)
	int32 NumLines = CachedGridLines;
	
	for (int32 i = -NumLines; i <= NumLines; ++i)
	{
		float Offset = i * GridSpacing;
		
		// Lines along X axis
		PDI->DrawLine(
			FVector(Offset, -GridSize, 0),
			FVector(Offset, GridSize, 0),
			GridColor,
			SDPG_World
		);
		
		// Lines along Y axis
		PDI->DrawLine(
			FVector(-GridSize, Offset, 0),
			FVector(GridSize, Offset, 0),
			GridColor,
			SDPG_World
		);
	}
	
	// Draw thicker center lines
	PDI->DrawLine(FVector(-GridSize, 0, 0), FVector(GridSize, 0, 0), FLinearColor::Green, SDPG_World, 2.0f);
	PDI->DrawLine(FVector(0, -GridSize, 0), FVector(0, GridSize, 0), FLinearColor::Red, SDPG_World, 2.0f);
}

void FStationViewportClient::UpdatePreviewComponents()
{
	if (!PreviewScene || !CurrentDesign)
	{
		ClearPreviewComponents();
		return;
	}

	// Track which modules exist in the current design
	TSet<FString> CurrentModuleIDs;
	for (const FModulePlacement& Module : CurrentDesign->Modules)
	{
		CurrentModuleIDs.Add(Module.ModuleID);
	}

	// Remove components for modules that no longer exist
	TArray<FString> ModulesToRemove;
	for (const auto& Pair : PreviewComponents)
	{
		if (!CurrentModuleIDs.Contains(Pair.Key))
		{
			ModulesToRemove.Add(Pair.Key);
		}
	}

	for (const FString& ModuleID : ModulesToRemove)
	{
		if (UStaticMeshComponent* Component = PreviewComponents[ModuleID])
		{
			PreviewScene->RemoveComponent(Component);
		}
		PreviewComponents.Remove(ModuleID);
		ModuleBlueprintPaths.Remove(ModuleID);
	}

	// Add or update components for current modules
	for (const FModulePlacement& Module : CurrentDesign->Modules)
	{
		UStaticMeshComponent* Component = PreviewComponents.FindRef(Module.ModuleID);
		
		if (!Component)
		{
			// Create new component for this module with proper Outer for garbage collection
			// Using GetTransientPackage() allows components to be GC'd when no longer referenced
			// Cleanup is ensured by ClearPreviewComponents() in destructor and SetStationDesign()
			Component = NewObject<UStaticMeshComponent>(GetTransientPackage());
			
			if (Component)
			{
				// Try to load the blueprint and extract the static mesh
				// Note: Synchronous loading is used for simplicity in this initial implementation
				// For production use with many modules, consider async loading with callbacks
				TArray<UMaterialInterface*> Materials;
				UStaticMesh* Mesh = LoadMeshFromBlueprintPath(Module.ModuleBlueprintPath, Materials);
				
				if (Mesh)
				{
					Component->SetStaticMesh(Mesh);
					
					// Copy materials
					for (int32 i = 0; i < Materials.Num(); ++i)
					{
						Component->SetMaterial(i, Materials[i]);
					}
				}
				
				// Only add the component to the preview scene if it has a valid static mesh
				if (Component->GetStaticMesh())
				{
					PreviewScene->AddComponent(Component, Module.Transform);
					PreviewComponents.Add(Module.ModuleID, Component);
					ModuleBlueprintPaths.Add(Module.ModuleID, Module.ModuleBlueprintPath);
				}
			}
		}
		else
		{
			// Update existing component when the module already has a preview component.
			// If the blueprint path (and thus the mesh) has changed, reload and update the mesh.
			FSoftClassPath* CachedPath = ModuleBlueprintPaths.Find(Module.ModuleID);
			
			if (!CachedPath || *CachedPath != Module.ModuleBlueprintPath)
			{
				// Blueprint path has changed, reload the mesh
				TArray<UMaterialInterface*> Materials;
				UStaticMesh* NewMesh = LoadMeshFromBlueprintPath(Module.ModuleBlueprintPath, Materials);
				
				if (NewMesh && Component->GetStaticMesh() != NewMesh)
				{
					Component->SetStaticMesh(NewMesh);
					
					// Update materials as well
					for (int32 i = 0; i < Materials.Num(); ++i)
					{
						Component->SetMaterial(i, Materials[i]);
					}
					
					// Update the cached path
					ModuleBlueprintPaths.Add(Module.ModuleID, Module.ModuleBlueprintPath);
				}
			}
			
			// Always update the transform to match the module's placement.
			Component->SetWorldTransform(Module.Transform);
		}
	}
}

void FStationViewportClient::ClearPreviewComponents()
{
	if (!PreviewScene)
	{
		return;
	}

	// Remove all components from the preview scene
	for (const auto& Pair : PreviewComponents)
	{
		if (UStaticMeshComponent* Component = Pair.Value)
		{
			PreviewScene->RemoveComponent(Component);
		}
	}

	PreviewComponents.Empty();
	ModuleBlueprintPaths.Empty();
}

UStaticMesh* FStationViewportClient::LoadMeshFromBlueprintPath(const FSoftClassPath& BlueprintPath, TArray<UMaterialInterface*>& OutMaterials)
{
	OutMaterials.Empty();
	
	if (BlueprintPath.IsNull())
	{
		return nullptr;
	}
	
	// Try to load the blueprint and extract the static mesh
	UClass* BlueprintClass = BlueprintPath.TryLoadClass<UObject>();
	if (!BlueprintClass)
	{
		return nullptr;
	}
	
	// Get the Class Default Object (CDO) to inspect components
	AActor* DefaultActor = Cast<AActor>(BlueprintClass->GetDefaultObject());
	if (!DefaultActor)
	{
		return nullptr;
	}
	
	// Try to find a static mesh component in the blueprint
	TArray<UStaticMeshComponent*> MeshComponents;
	DefaultActor->GetComponents<UStaticMeshComponent>(MeshComponents);
	
	if (MeshComponents.Num() > 0 && MeshComponents[0]->GetStaticMesh())
	{
		// Copy materials
		for (int32 i = 0; i < MeshComponents[0]->GetNumMaterials(); ++i)
		{
			OutMaterials.Add(MeshComponents[0]->GetMaterial(i));
		}
		
		return MeshComponents[0]->GetStaticMesh();
	}
	
	return nullptr;
}
