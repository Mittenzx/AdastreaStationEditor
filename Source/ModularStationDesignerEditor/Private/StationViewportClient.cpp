// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationViewportClient.h"
#include "VisualizationSystem.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "PreviewScene.h"
#include "CanvasItem.h"
#include "CanvasTypes.h"
#include "Engine/Canvas.h"

FStationViewportClient::FStationViewportClient(FPreviewScene* InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewportWidget)
	: FEditorViewportClient(nullptr, InPreviewScene, InEditorViewportWidget)
	, CurrentDesign(nullptr)
	, AnimationTime(0.0f)
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
}

FStationViewportClient::~FStationViewportClient()
{
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
}

void FStationViewportClient::DrawModules(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	if (!CurrentDesign)
	{
		return;
	}

	// Draw each module as a box with color coding
	for (const FModulePlacement& Module : CurrentDesign->Modules)
	{
		FVector Location = Module.Transform.GetLocation();
		FRotator Rotation = Module.Transform.Rotator();
		
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
		FTransform ModuleTransform = Module.Transform;
		FBox Box(Location - BoxExtent, Location + BoxExtent);
		DrawWireBox(PDI, Box, ModuleColor, SDPG_World);
		
		// Draw orientation indicator
		float AxisLength = 75.0f;
		FVector XAxis = ModuleTransform.TransformVector(FVector::ForwardVector) * AxisLength;
		FVector YAxis = ModuleTransform.TransformVector(FVector::RightVector) * AxisLength;
		FVector ZAxis = ModuleTransform.TransformVector(FVector::UpVector) * AxisLength;
		
		PDI->DrawLine(Location, Location + XAxis, FLinearColor::Red, SDPG_World, 2.0f);
		PDI->DrawLine(Location, Location + YAxis, FLinearColor::Green, SDPG_World, 2.0f);
		PDI->DrawLine(Location, Location + ZAxis, FLinearColor::Blue, SDPG_World, 2.0f);
	}
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
	
	int32 NumLines = FMath::CeilToInt(GridSize / GridSpacing);
	
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
