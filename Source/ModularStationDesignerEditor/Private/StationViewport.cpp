// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationViewport.h"
#include "ModuleDragDropOp.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "StationViewport"

void SStationViewport::Construct(const FArguments& InArgs)
{
	CurrentDesign = FStationDesign();
	SelectedModuleIndex = INDEX_NONE;

	ChildSlot
	[
		CreateViewportContent()
	];
}

TSharedRef<SWidget> SStationViewport::CreateViewportContent()
{
	return SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
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
				.Font(FEditorStyle::GetFontStyle("BoldFont"))
			]

			// Viewport content area
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
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
							.Font(FEditorStyle::GetFontStyle("LargeFont"))
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
									CurrentDesign.Modules.Num()));
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

	CurrentDesign.Modules.Add(NewPlacement);

	UE_LOG(LogTemp, Log, TEXT("Added module: %s at location %s"),
		*ModuleInfo.Name, *Transform.GetLocation().ToString());
}

void SStationViewport::RemoveSelectedModule()
{
	if (SelectedModuleIndex >= 0 && SelectedModuleIndex < CurrentDesign.Modules.Num())
	{
		CurrentDesign.Modules.RemoveAt(SelectedModuleIndex);
		SelectedModuleIndex = INDEX_NONE;
		UE_LOG(LogTemp, Log, TEXT("Removed selected module"));
	}
}

void SStationViewport::ClearModules()
{
	CurrentDesign.Modules.Empty();
	SelectedModuleIndex = INDEX_NONE;
	UE_LOG(LogTemp, Log, TEXT("Cleared all modules"));
}

void SStationViewport::SetCurrentDesign(const FStationDesign& Design)
{
	CurrentDesign = Design;
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
		FVector DropLocation(LocalPos.X * 10.0f, LocalPos.Y * 10.0f, 0.0f); // Scale for 3D space
		
		FTransform DropTransform;
		DropTransform.SetLocation(DropLocation);

		// Add the module
		AddModule(*ModuleDragOp->ModuleInfo, DropTransform);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE
