// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StationDesignerTypes.h"

class UBlueprint;

/**
 * Information about a discovered module
 */
struct FModuleInfo
{
	FString Name;
	FString BlueprintPath;
	FString ModuleType;
	EStationModuleGroup ModuleGroup;
	float PowerConsumption;
	TSharedPtr<struct FSlateBrush> Thumbnail;

	FModuleInfo()
		: Name(TEXT(""))
		, BlueprintPath(TEXT(""))
		, ModuleType(TEXT(""))
		, ModuleGroup(EStationModuleGroup::Other)
		, PowerConsumption(0.0f)
	{
	}
};

/**
 * Module discovery system - scans for Adastrea station modules
 */
class FModuleDiscovery
{
public:
	// Discover all station modules in the project
	static TArray<FModuleInfo> DiscoverModules();
	
	// Discover modules with filter
	static TArray<FModuleInfo> DiscoverModulesFiltered(EStationModuleGroup GroupFilter);

private:
	// Helper to extract module metadata from Blueprint
	static FModuleInfo ExtractModuleInfo(UBlueprint* Blueprint);
	
	// Helper to determine module group from name
	static EStationModuleGroup DetermineModuleGroup(const FString& ModuleName);
};
