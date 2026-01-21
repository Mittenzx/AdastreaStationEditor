// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModulePalette.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SButton.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "ModulePalette"

void SModulePalette::Construct(const FArguments& InArgs)
{
	CurrentGroupFilter = EStationModuleGroup::All;
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
		]
	];
}

void SModulePalette::RefreshModuleList()
{
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
							.Justification(ETextJustify::Center)
						]
					]
				]
				
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
				]
			]
		];
}

void SModulePalette::OnSearchTextChanged(const FText& Text)
{
	SearchText = Text.ToString();
	ApplyFilters();
}

void SModulePalette::OnSearchTextCommitted(const FText& Text, ETextCommit::Type CommitType)
{
	SearchText = Text.ToString();
	ApplyFilters();
}

void SModulePalette::OnGroupFilterChanged(EStationModuleGroup NewFilter)
{
	CurrentGroupFilter = NewFilter;
	ApplyFilters();
}

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
}

#undef LOCTEXT_NAMESPACE
