// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StationDesignerTypes.h"

class UBlueprint;

/**
 * Station exporter - handles Blueprint generation and file I/O
 */
class FStationExporter
{
public:
	// Export station design to Adastrea Blueprint
	static bool ExportToBlueprintAsset(
		const FStationDesign& Design,
		const FString& TargetPackagePath,
		const FString& AssetName,
		FString& OutErrorMessage
	);
	
	// Save design to JSON file
	static bool SaveDesignToFile(
		const FStationDesign& Design,
		const FString& FilePath
	);
	
	// Load design from JSON file
	static bool LoadDesignFromFile(
		const FString& FilePath,
		FStationDesign& OutDesign
	);

private:
	static UBlueprint* CreateBlueprintAsset(const FString& PackagePath, const FString& AssetName);
	static void AddChildActorComponents(UBlueprint* Blueprint, const FStationDesign& Design);
	static void ConfigureStationDefaults(UBlueprint* Blueprint, const FStationDesign& Design);
};
