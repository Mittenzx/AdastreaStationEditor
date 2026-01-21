// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "StationDesignerTypes.h"

class SModulePalette;
class SStationViewport;

/**
 * Main Station Designer Window (Slate UI)
 * Provides the complete interface for designing stations
 */
class SStationDesignerWindow : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStationDesignerWindow) {}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	// Current station design
	FStationDesign CurrentDesign;

	// UI Components
	TSharedPtr<SModulePalette> ModulePalette;
	TSharedPtr<SStationViewport> StationViewport;

	// UI event handlers
	FReply OnNewStation();
	FReply OnLoadStation();
	FReply OnSaveStation();
	FReply OnExportStation();
	FReply OnValidateStation();

	// Create UI sections
	TSharedRef<SWidget> CreateToolbar();
	TSharedRef<SWidget> CreateMainContent();
	TSharedRef<SWidget> CreateStatusBar();
};
