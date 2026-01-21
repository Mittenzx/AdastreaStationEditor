// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModulePalette.h"
#include "ModuleDragDropOp.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "ModulePalette"

void SModulePalette::Construct(const FArguments& InArgs)
{
	CurrentGroupFilter = EStationModuleGroup::All;
	CurrentSearchText = FString();

	// Load modules
	RefreshModuleList();

	ChildSlot
	[
		SNew(SVerticalBox)

		// Search box
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4.0f)
		[
			SAssignNew(SearchBox, SSearchBox)
			.OnTextChanged(this, &SModulePalette::OnSearchTextChanged)
			.HintText(LOCTEXT("SearchHint", "Search modules..."))
		]

		// Group filter buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4.0f)
		[
			CreateGroupFilters()
		]

		// Module list
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(4.0f)
		[
			SAssignNew(ModuleListView, SListView<TSharedPtr<FModuleInfo>>)
			.ListItemsSource(&FilteredModules)
			.OnGenerateRow(this, &SModulePalette::OnGenerateModuleRow)
			.SelectionMode(ESelectionMode::Single)
		]
	];
}

void SModulePalette::RefreshModuleList()
{
	// Discover modules
	TArray<FModuleInfo> DiscoveredModules = FModuleDiscovery::DiscoverModules();
	
	// Convert to shared pointers
	AllModules.Empty();
	for (const FModuleInfo& Module : DiscoveredModules)
	{
		AllModules.Add(MakeShared<FModuleInfo>(Module));
	}

	// Apply filters
	ApplyFilters();
}

TSharedRef<ITableRow> SModulePalette::OnGenerateModuleRow(
	TSharedPtr<FModuleInfo> Item,
	const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedPtr<FModuleInfo> ModuleInfoPtr = Item;
	
	return SNew(STableRow<TSharedPtr<FModuleInfo>>, OwnerTable)
		.OnDragDetected_Lambda([this, ModuleInfoPtr](const FGeometry& Geo, const FPointerEvent& Event) {
			return OnModuleDragDetected(Geo, Event, ModuleInfoPtr);
		})
		[
			SNew(SBorder)
			.Padding(4.0f)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
			[
				SNew(SHorizontalBox)

				// Thumbnail (placeholder for now)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(2.0f)
				[
					SNew(SBox)
					.WidthOverride(48.0f)
					.HeightOverride(48.0f)
					[
						SNew(SBorder)
						.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("?")))
							.Justification(ETextJustify::Center)
						]
					]
				]

				// Module info
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(4.0f)
				.VAlign(VAlign_Center)
				[
					SNew(SVerticalBox)

					// Module name
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(FText::FromString(Item->Name))
						.Font(FEditorStyle::GetFontStyle("BoldFont"))
					]

					// Module type and power
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString::Printf(TEXT("%s | Power: %.1f MW"),
							*Item->ModuleType, Item->PowerConsumption)))
						.Font(FEditorStyle::GetFontStyle("SmallFont"))
					]
				]
			]
		];
}

FReply SModulePalette::OnModuleDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, TSharedPtr<FModuleInfo> ModuleInfo)
{
	if (ModuleInfo.IsValid() && MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		TSharedRef<FModuleDragDropOp> DragDropOp = FModuleDragDropOp::New(ModuleInfo);
		return FReply::Handled().BeginDragDrop(DragDropOp);
	}
	
	return FReply::Unhandled();
}

void SModulePalette::OnSearchTextChanged(const FText& InFilterText)
{
	CurrentSearchText = InFilterText.ToString();
	ApplyFilters();
}

void SModulePalette::OnGroupFilterChanged(EStationModuleGroup NewFilter)
{
	CurrentGroupFilter = NewFilter;
	ApplyFilters();
}

void SModulePalette::ApplyFilters()
{
	FilteredModules.Empty();

	for (const TSharedPtr<FModuleInfo>& Module : AllModules)
	{
		// Apply group filter
		if (CurrentGroupFilter != EStationModuleGroup::All &&
			Module->ModuleGroup != CurrentGroupFilter)
		{
			continue;
		}

		// Apply search filter
		if (!CurrentSearchText.IsEmpty())
		{
			if (!Module->Name.Contains(CurrentSearchText) &&
				!Module->ModuleType.Contains(CurrentSearchText))
			{
				continue;
			}
		}

		FilteredModules.Add(Module);
	}

	// Refresh list view
	if (ModuleListView.IsValid())
	{
		ModuleListView->RequestListRefresh();
	}
}

TSharedRef<SWidget> SModulePalette::CreateGroupFilters()
{
	return SNew(SScrollBox)
		.Orientation(Orient_Horizontal)
		
		+ SScrollBox::Slot()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("AllFilter", "All"))
			.OnClicked_Lambda([this]() {
				OnGroupFilterChanged(EStationModuleGroup::All);
				return FReply::Handled();
			})
		]
		
		+ SScrollBox::Slot()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("DockingFilter", "Docking"))
			.OnClicked_Lambda([this]() {
				OnGroupFilterChanged(EStationModuleGroup::Docking);
				return FReply::Handled();
			})
		]
		
		+ SScrollBox::Slot()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("PowerFilter", "Power"))
			.OnClicked_Lambda([this]() {
				OnGroupFilterChanged(EStationModuleGroup::Power);
				return FReply::Handled();
			})
		]
		
		+ SScrollBox::Slot()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("StorageFilter", "Storage"))
			.OnClicked_Lambda([this]() {
				OnGroupFilterChanged(EStationModuleGroup::Storage);
				return FReply::Handled();
			})
		]
		
		+ SScrollBox::Slot()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("ProcessingFilter", "Processing"))
			.OnClicked_Lambda([this]() {
				OnGroupFilterChanged(EStationModuleGroup::Processing);
				return FReply::Handled();
			})
		]
		
		+ SScrollBox::Slot()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("DefenceFilter", "Defence"))
			.OnClicked_Lambda([this]() {
				OnGroupFilterChanged(EStationModuleGroup::Defence);
				return FReply::Handled();
			})
		];
}

#undef LOCTEXT_NAMESPACE
