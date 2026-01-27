// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationViewport.h"
#include "StationViewportClient.h"
#include "ModuleDragDropOp.h"
#include "PreviewScene.h"
#include "SceneView.h"

#define LOCTEXT_NAMESPACE "StationViewport"

// Coordinate space conversion from 2D screen space to 3D world space
static constexpr float ScreenToWorldScale = 10.0f;

SStationViewport::SStationViewport()
	: ExternalDesign(nullptr)
	, SelectedModuleIndex(INDEX_NONE)
{
}

SStationViewport::~SStationViewport()
{
}

void SStationViewport::Construct(const FArguments& InArgs)
{
	// Store pointer to external design if provided
	ExternalDesign = InArgs._StationDesign;
	
	// Initialize internal design (used as fallback)
	InternalDesign = FStationDesign();
	
	SelectedModuleIndex = INDEX_NONE;

	// Create preview scene for rendering
	PreviewScene = MakeShared<FPreviewScene>(FPreviewScene::ConstructionValues());

	// Call parent Construct to set up the viewport
	SEditorViewport::Construct(SEditorViewport::FArguments());
}

TSharedRef<FEditorViewportClient> SStationViewport::MakeEditorViewportClient()
{
	// Create viewport client for 3D rendering
	ViewportClient = MakeShared<FStationViewportClient>(PreviewScene.Get(), SharedThis(this));
	
	// Set the station design to visualize
	ViewportClient->SetStationDesign(&GetActiveDesign());
	
	return ViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SStationViewport::MakeViewportToolbar()
{
	// Return nullptr for now - can add toolbar later with view options
	return nullptr;
}

void SStationViewport::AddModule(const FModuleInfo& ModuleInfo, const FTransform& Transform)
{
	FModulePlacement NewPlacement;
	NewPlacement.ModuleID = FGuid::NewGuid().ToString();
	NewPlacement.ModuleBlueprintPath = FSoftClassPath(ModuleInfo.BlueprintPath);
	NewPlacement.Transform = Transform;
	NewPlacement.ComponentName = ModuleInfo.Name;

	GetActiveDesign().Modules.Add(NewPlacement);

	// Refresh the viewport to show the new module
	RefreshViewport();

	UE_LOG(LogTemp, Log, TEXT("Added module: %s at location %s"),
		*ModuleInfo.Name, *Transform.GetLocation().ToString());
}

void SStationViewport::RemoveSelectedModule()
{
	if (SelectedModuleIndex >= 0 && SelectedModuleIndex < GetActiveDesign().Modules.Num())
	{
		GetActiveDesign().Modules.RemoveAt(SelectedModuleIndex);
		SelectedModuleIndex = INDEX_NONE;
		RefreshViewport();
		UE_LOG(LogTemp, Log, TEXT("Removed selected module"));
	}
}

void SStationViewport::ClearModules()
{
	GetActiveDesign().Modules.Empty();
	SelectedModuleIndex = INDEX_NONE;
	RefreshViewport();
	UE_LOG(LogTemp, Log, TEXT("Cleared all modules"));
}

void SStationViewport::SetCurrentDesign(const FStationDesign& Design)
{
	if (ExternalDesign)
	{
		*ExternalDesign = Design;
	}
	else
	{
		InternalDesign = Design;
	}
	SelectedModuleIndex = INDEX_NONE;
	RefreshViewport();
}

void SStationViewport::RefreshViewport()
{
	// Update viewport client with current design
	if (ViewportClient.IsValid())
	{
		ViewportClient->SetStationDesign(&GetActiveDesign());
		ViewportClient->Invalidate();
	}
}

FReply SStationViewport::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	// Visual feedback during drag
	return FReply::Handled();
}

FReply SStationViewport::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	TSharedPtr<FModuleDragDropOp> ModuleDragOp = DragDropEvent.GetOperationAs<FModuleDragDropOp>();
	if (ModuleDragOp.IsValid() && ModuleDragOp->ModuleInfo.IsValid())
	{
		// Calculate drop position in 3D space
		FVector2D LocalPos = MyGeometry.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition());
		
		// Default drop at origin if viewport client not ready
		FVector DropLocation(0.0f, 0.0f, 0.0f);
		
		if (ViewportClient.IsValid() && ViewportClient->Viewport)
		{
			// Project screen position to 3D world space at Z=0 plane
			FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
				ViewportClient->Viewport,
				ViewportClient->GetScene(),
				ViewportClient->EngineShowFlags)
				.SetRealtimeUpdate(true));
			
			FSceneView* View = ViewportClient->CalcSceneView(&ViewFamily);
			if (View)
			{
				FVector WorldOrigin, WorldDirection;
				View->DeprojectFVector2D(LocalPos, WorldOrigin, WorldDirection);
				
				// Intersect with Z=0 plane
				float IntersectT = -WorldOrigin.Z / WorldDirection.Z;
				if (IntersectT > 0)
				{
					DropLocation = WorldOrigin + WorldDirection * IntersectT;
				}
			}
		}
		else
		{
			// Fallback: Simple 2D to 3D conversion
			DropLocation = FVector(LocalPos.X * ScreenToWorldScale, LocalPos.Y * ScreenToWorldScale, 0.0f);
		}
		
		FTransform DropTransform;
		DropTransform.SetLocation(DropLocation);

		// Add the module
		AddModule(*ModuleDragOp->ModuleInfo, DropTransform);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE
