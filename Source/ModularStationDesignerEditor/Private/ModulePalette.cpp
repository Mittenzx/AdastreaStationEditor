// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModulePalette.h"
<<<<<<< HEAD
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"
=======
#include "ModuleDragDropOp.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Styling/AppStyle.h"
>>>>>>> origin/main

#define LOCTEXT_NAMESPACE "ModulePalette"

void SModulePalette::Construct(const FArguments& InArgs)
{
	CurrentGroupFilter = EStationModuleGroup::All;
<<<<<<< HEAD
	SearchText = TEXT("");
	SelectedModule = nullptr;
	
	// Initialize module list
	RefreshModuleList();
	
	ChildSlot
	[
		SNew(SVerticalBox)
		
		// Search bar
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f)
		[
			CreateSearchBar()
		]
		
		// Filter bar
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f, 0.0f)
		[
			CreateFilterBar()
		]
		
		// Module grid
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		.Padding(5.0f)
		[
			CreateModuleGrid()
		]
		
		// Status footer
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(STextBlock)
			.Text_Lambda([this]()
			{
				return FText::Format(
					LOCTEXT("ModuleCount", "{0} modules available"),
					FText::AsNumber(FilteredModules.Num())
				);
			})
=======
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
>>>>>>> origin/main
		]
	];
}

void SModulePalette::RefreshModuleList()
{
<<<<<<< HEAD
	AvailableModules.Empty();
	
	// Discover modules from the project
	TArray<FModuleInfo> Modules = FModuleDiscovery::DiscoverModules();
	
	// Convert to shared pointers for UI
	for (const FModuleInfo& Module : Modules)
	{
		AvailableModules.Add(MakeShared<FModuleInfo>(Module));
	}
	
	// Apply current filters
	ApplyFilters();
}

void SModulePalette::SetGroupFilter(EStationModuleGroup Filter)
{
	if (CurrentGroupFilter != Filter)
	{
		CurrentGroupFilter = Filter;
		ApplyFilters();
	}
}

TSharedPtr<FModuleInfo> SModulePalette::GetSelectedModule() const
{
	return SelectedModule;
}

TSharedRef<SWidget> SModulePalette::CreateSearchBar()
{
	return SNew(SSearchBox)
		.HintText(LOCTEXT("SearchHint", "Search modules..."))
		.OnTextChanged(this, &SModulePalette::OnSearchTextChanged)
		.OnTextCommitted(this, &SModulePalette::OnSearchTextCommitted);
}

TSharedRef<SWidget> SModulePalette::CreateFilterBar()
{
	return SNew(SHorizontalBox)
		
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("AllFilter", "All"))
			.OnClicked_Lambda([this]() { OnGroupFilterChanged(EStationModuleGroup::All); return FReply::Handled(); })
		]
		
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("DockingFilter", "Docking"))
			.OnClicked_Lambda([this]() { OnGroupFilterChanged(EStationModuleGroup::Docking); return FReply::Handled(); })
		]
		
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("PowerFilter", "Power"))
			.OnClicked_Lambda([this]() { OnGroupFilterChanged(EStationModuleGroup::Power); return FReply::Handled(); })
		]
		
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("StorageFilter", "Storage"))
			.OnClicked_Lambda([this]() { OnGroupFilterChanged(EStationModuleGroup::Storage); return FReply::Handled(); })
		]
		
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("ProcessingFilter", "Processing"))
			.OnClicked_Lambda([this]() { OnGroupFilterChanged(EStationModuleGroup::Processing); return FReply::Handled(); })
		]
		
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f)
		[
			SNew(SButton)
			.Text(LOCTEXT("DefenceFilter", "Defence"))
			.OnClicked_Lambda([this]() { OnGroupFilterChanged(EStationModuleGroup::Defence); return FReply::Handled(); })
		];
}

TSharedRef<SWidget> SModulePalette::CreateModuleGrid()
{
	TSharedRef<SScrollBox> ScrollBox = SNew(SScrollBox)
		.Orientation(Orient_Vertical);
	
	TSharedRef<SWrapBox> WrapBox = SNew(SWrapBox)
		.UseAllottedSize(true)
		.InnerSlotPadding(FVector2D(5.0f, 5.0f));
	
	// Add module tiles
	for (TSharedPtr<FModuleInfo> Module : FilteredModules)
	{
		WrapBox->AddSlot()
		.Padding(5.0f)
		[
			CreateModuleTile(Module)
		];
	}
	
	ScrollBox->AddSlot()
	[
		WrapBox
	];
	
	ModuleGridWidget = ScrollBox;
	return ScrollBox;
}

TSharedRef<SWidget> SModulePalette::CreateModuleTile(TSharedPtr<FModuleInfo> Module)
{
	return SNew(SBorder)
		.BorderBackgroundColor(FLinearColor(0.2f, 0.2f, 0.2f, 1.0f))
		.Padding(10.0f)
		[
			SNew(SButton)
			.ContentPadding(5.0f)
			.OnClicked(this, &SModulePalette::OnModuleClicked, Module)
			[
				SNew(SVerticalBox)
				
				// Module icon/thumbnail (placeholder)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				[
					SNew(SBox)
					.WidthOverride(64.0f)
					.HeightOverride(64.0f)
					[
						SNew(SBorder)
						.BorderBackgroundColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f))
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("🔷")))
							.Font(FCoreStyle::GetDefaultFontStyle("Regular", 32))
=======
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
			.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
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
						.BorderImage(FAppStyle::GetBrush("ToolPanel.DarkGroupBorder"))
						[
							SNew(STextBlock)
							.Text(FText::FromString(TEXT("?")))
>>>>>>> origin/main
							.Justification(ETextJustify::Center)
						]
					]
				]
<<<<<<< HEAD
				
				// Module name
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0.0f, 5.0f, 0.0f, 0.0f)
				[
					SNew(STextBlock)
					.Text(FText::FromString(Module->Name))
					.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
					.Justification(ETextJustify::Center)
					.AutoWrapText(true)
				]
				
				// Module type
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::FromString(Module->ModuleType))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f, 1.0f))
				]
				
				// Power consumption
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
					.Text(FText::Format(
						LOCTEXT("PowerDisplay", "{0} MW"),
						FText::AsNumber(Module->PowerConsumption)
					))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 8))
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(
						Module->PowerConsumption < 0.0f ? 
						FLinearColor(0.2f, 0.8f, 0.2f, 1.0f) :  // Green for generators
						FLinearColor(0.8f, 0.8f, 0.2f, 1.0f)    // Yellow for consumers
					)
=======

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
						.Font(FAppStyle::GetFontStyle("BoldFont"))
					]

					// Module type and power
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(STextBlock)
						.Text(FText::FromString(FString::Printf(TEXT("%s | Power: %.1f MW"),
							*Item->ModuleType, Item->PowerConsumption)))
						.Font(FAppStyle::GetFontStyle("SmallFont"))
					]
>>>>>>> origin/main
				]
			]
		];
}

<<<<<<< HEAD
void SModulePalette::OnSearchTextChanged(const FText& Text)
{
	SearchText = Text.ToString();
	ApplyFilters();
}

void SModulePalette::OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
	SearchText = Text.ToString();
=======
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
>>>>>>> origin/main
	ApplyFilters();
}

void SModulePalette::OnGroupFilterChanged(EStationModuleGroup NewFilter)
{
	CurrentGroupFilter = NewFilter;
	ApplyFilters();
}

<<<<<<< HEAD
FReply SModulePalette::OnModuleClicked(TSharedPtr<FModuleInfo> Module)
{
	SelectedModule = Module;
	UE_LOG(LogTemp, Log, TEXT("Module selected: %s"), *Module->Name);
	return FReply::Handled();
}

FReply SModulePalette::OnModuleDragDetected(const FGeometry& Geometry, const FPointerEvent& PointerEvent, TSharedPtr<FModuleInfo> Module)
{
	// This would be called when drag-and-drop is fully implemented
	// For now, just select the module
	SelectedModule = Module;
	UE_LOG(LogTemp, Log, TEXT("Drag started for module: %s"), *Module->Name);
	return FReply::Handled();
}

void SModulePalette::ApplyFilters()
{
	FilteredModules.Empty();
	
	for (TSharedPtr<FModuleInfo> Module : AvailableModules)
	{
		if (PassesFilter(Module))
		{
			FilteredModules.Add(Module);
		}
	}
	
	// Refresh the UI if the grid widget exists
	// In a full implementation, this would regenerate the grid
	UE_LOG(LogTemp, Verbose, TEXT("Filters applied: %d modules match"), FilteredModules.Num());
}

bool SModulePalette::PassesFilter(TSharedPtr<FModuleInfo> Module) const
{
	if (!Module.IsValid())
	{
		return false;
	}
	
	// Group filter
	if (CurrentGroupFilter != EStationModuleGroup::All && Module->ModuleGroup != CurrentGroupFilter)
	{
		return false;
	}
	
	// Search text filter
	if (!SearchText.IsEmpty())
	{
		if (!Module->Name.Contains(SearchText) && 
		    !Module->ModuleType.Contains(SearchText))
		{
			return false;
		}
	}
	
	return true;
=======
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
			if (!Module->Name.Contains(CurrentSearchText, ESearchCase::IgnoreCase) &&
				!Module->ModuleType.Contains(CurrentSearchText, ESearchCase::IgnoreCase))
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
>>>>>>> origin/main
}

#undef LOCTEXT_NAMESPACE
