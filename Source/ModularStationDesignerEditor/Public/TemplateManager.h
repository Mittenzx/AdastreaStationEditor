// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StationDesignerTypes.h"

/**
 * Template manager - handles station design templates
 */
class FTemplateManager
{
public:
	// Load all available templates
	static TArray<FStationDesign> LoadTemplates();
	
	// Save design as template
	static bool SaveAsTemplate(const FStationDesign& Design, const FString& TemplateName, const FString& Description);
	
	// Load template by name
	static bool LoadTemplate(const FString& TemplateName, FStationDesign& OutDesign);
	
	// Get template info
	struct FTemplateInfo
	{
		FString Name;
		FString Description;
		FString FilePath;
		int32 ModuleCount;
		FDateTime CreatedDate;
	};
	
	static TArray<FTemplateInfo> GetTemplateList();
	
	// Create built-in starter templates
	static void CreateStarterTemplates();

private:
	static FString GetTemplatesDirectory();
	static FStationDesign CreateTradeOutpostTemplate();
	static FStationDesign CreateMiningStationTemplate();
	static FStationDesign CreateResearchFacilityTemplate();
	static FStationDesign CreateMilitaryBaseTemplate();
	static FStationDesign CreateTradingHubTemplate();
};
