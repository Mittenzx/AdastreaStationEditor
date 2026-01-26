// Copyright Epic Games, Inc. All Rights Reserved.

// NOTE: This is a PLACEHOLDER implementation of the 3D viewport.
// It currently displays text-based instructions and module counts,
// but does NOT render actual 3D geometry.
//
// To implement real 3D visualization:
// 1. Replace SStationViewport to inherit from SEditorViewport
// 2. Create FStationViewportClient derived from FEditorViewportClient
// 3. Add UnrealEd module dependency in .Build.cs
// 4. Implement OnDraw to render modules, connections, and visual indicators
// 5. Add camera controls (orbit, pan, zoom)
// 6. Integrate with FVisualizationSystem for power flow, connections, etc.

#include "StationViewport.h"
#include "ModuleDragDropOp.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "StationViewport"

// Coordinate space conversion from 2D screen space to 3D world space
static constexpr float ScreenToWorldScale = 10.0f;

void SStationViewport::Construct(const FArguments& InArgs)
{
	// Store pointer to external design if provided
	ExternalDesign = InArgs._StationDesign;
	
	// Initialize internal design (used as fallback)
	InternalDesign = FStationDesign();
	
	SelectedModuleIndex = INDEX_NONE;

	ChildSlot
	[
		CreateViewportContent()
	];
}

TSharedRef<SWidget> SStationViewport::CreateViewportContent()
{
	return SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
		.Padding(10.0f)
		[
			SNew(SVerticalBox)

			// Viewport header
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 0.0f, 0.0f, 10.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ViewportTitle", "3D Station Viewport"))
				.Font(FAppStyle::GetFontStyle("BoldFont"))
			]

			// Viewport content area
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
				.Padding(20.0f)
				[
					SNew(SBox)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					[
						SNew(SVerticalBox)

						// Main viewport info
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(STextBlock)
							.Text(LOCTEXT("ViewportPlaceholder", "3D Viewport Visualization"))
							.Justification(ETextJustify::Center)
							.Font(FAppStyle::GetFontStyle("LargeFont"))
						]

						// Instructions
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 10.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text(LOCTEXT("ViewportInstructions", 
								"Drag modules from the palette to place them here.\n"
								"Click to select | Right-click for options\n"
								"Camera: WASD to move | RMB to rotate"))
							.Justification(ETextJustify::Center)
							.AutoWrapText(true)
						]

						// Module count
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 10.0f, 0.0f, 0.0f)
						[
							SNew(STextBlock)
							.Text_Lambda([this]() {
								return FText::FromString(FString::Printf(
									TEXT("Modules Placed: %d"),
									GetActiveDesign().Modules.Num()));
							})
							.Justification(ETextJustify::Center)
						]
					]
				]
			]
		];
}

void SStationViewport::AddModule(const FModuleInfo& ModuleInfo, const FTransform& Transform)
{
	FModulePlacement NewPlacement;
	NewPlacement.ModuleID = FGuid::NewGuid().ToString();
	NewPlacement.ModuleBlueprintPath = FSoftClassPath(ModuleInfo.BlueprintPath);
	NewPlacement.Transform = Transform;
	NewPlacement.ComponentName = ModuleInfo.Name;

	GetActiveDesign().Modules.Add(NewPlacement);

	UE_LOG(LogTemp, Log, TEXT("Added module: %s at location %s"),
		*ModuleInfo.Name, *Transform.GetLocation().ToString());
}

void SStationViewport::RemoveSelectedModule()
{
	if (SelectedModuleIndex >= 0 && SelectedModuleIndex < GetActiveDesign().Modules.Num())
	{
		GetActiveDesign().Modules.RemoveAt(SelectedModuleIndex);
		SelectedModuleIndex = INDEX_NONE;
		UE_LOG(LogTemp, Log, TEXT("Removed selected module"));
	}
}

void SStationViewport::ClearModules()
{
	GetActiveDesign().Modules.Empty();
	SelectedModuleIndex = INDEX_NONE;
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
		// Calculate drop position
		FVector2D LocalPos = MyGeometry.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition());
		FVector DropLocation(LocalPos.X * ScreenToWorldScale, LocalPos.Y * ScreenToWorldScale, 0.0f);
		
		FTransform DropTransform;
		DropTransform.SetLocation(DropLocation);

		// Add the module
		AddModule(*ModuleDragOp->ModuleInfo, DropTransform);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE
