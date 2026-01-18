// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModuleDiscovery.h"
#include "Engine/Blueprint.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/UObjectIterator.h"

TArray<FModuleInfo> FModuleDiscovery::DiscoverModules()
{
	TArray<FModuleInfo> Modules;

	// Get asset registry
	FAssetRegistryModule& AssetRegistryModule = 
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	// Find all Blueprint assets
	TArray<FAssetData> BlueprintAssets;
	AssetRegistry.GetAssetsByClass(UBlueprint::StaticClass()->GetClassPathName(), BlueprintAssets);

	// Filter for station module Blueprints
	for (const FAssetData& AssetData : BlueprintAssets)
	{
		// Check if this looks like a station module
		FString AssetName = AssetData.AssetName.ToString();
		if (AssetName.Contains(TEXT("Module")) || 
			AssetName.Contains(TEXT("Station")) ||
			AssetName.Contains(TEXT("Docking")) ||
			AssetName.Contains(TEXT("Reactor")) ||
			AssetName.Contains(TEXT("Cargo")))
		{
			UBlueprint* Blueprint = Cast<UBlueprint>(AssetData.GetAsset());
			if (Blueprint)
			{
				FModuleInfo Info = ExtractModuleInfo(Blueprint);
				Info.BlueprintPath = AssetData.GetObjectPathString();
				Modules.Add(Info);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Discovered %d station modules"), Modules.Num());
	return Modules;
}

TArray<FModuleInfo> FModuleDiscovery::DiscoverModulesFiltered(EStationModuleGroup GroupFilter)
{
	TArray<FModuleInfo> AllModules = DiscoverModules();
	
	if (GroupFilter == EStationModuleGroup::All)
	{
		return AllModules;
	}

	TArray<FModuleInfo> FilteredModules;
	for (const FModuleInfo& Module : AllModules)
	{
		if (Module.ModuleGroup == GroupFilter)
		{
			FilteredModules.Add(Module);
		}
	}

	return FilteredModules;
}

FModuleInfo FModuleDiscovery::ExtractModuleInfo(UBlueprint* Blueprint)
{
	FModuleInfo Info;

	if (!Blueprint)
	{
		return Info;
	}

	Info.Name = Blueprint->GetName();
	Info.ModuleType = Info.Name;
	Info.ModuleGroup = DetermineModuleGroup(Info.Name);
	
	// Default power values based on module type
	if (Info.Name.Contains(TEXT("Reactor")))
	{
		Info.PowerConsumption = -500.0f; // Generates power
	}
	else if (Info.Name.Contains(TEXT("Solar")))
	{
		Info.PowerConsumption = -200.0f; // Generates power
	}
	else if (Info.Name.Contains(TEXT("Docking")))
	{
		Info.PowerConsumption = 50.0f;
	}
	else if (Info.Name.Contains(TEXT("Marketplace")))
	{
		Info.PowerConsumption = 40.0f;
	}
	else
	{
		Info.PowerConsumption = 30.0f;
	}

	return Info;
}

EStationModuleGroup FModuleDiscovery::DetermineModuleGroup(const FString& ModuleName)
{
	if (ModuleName.Contains(TEXT("Docking")))
	{
		return EStationModuleGroup::Docking;
	}
	else if (ModuleName.Contains(TEXT("Reactor")) || ModuleName.Contains(TEXT("Solar")) || ModuleName.Contains(TEXT("Power")))
	{
		return EStationModuleGroup::Power;
	}
	else if (ModuleName.Contains(TEXT("Cargo")) || ModuleName.Contains(TEXT("Storage")))
	{
		return EStationModuleGroup::Storage;
	}
	else if (ModuleName.Contains(TEXT("Fabrication")) || ModuleName.Contains(TEXT("Processing")))
	{
		return EStationModuleGroup::Processing;
	}
	else if (ModuleName.Contains(TEXT("Turret")) || ModuleName.Contains(TEXT("Shield")) || ModuleName.Contains(TEXT("Defence")))
	{
		return EStationModuleGroup::Defence;
	}
	else if (ModuleName.Contains(TEXT("Habitation")) || ModuleName.Contains(TEXT("Barracks")))
	{
		return EStationModuleGroup::Habitation;
	}
	else if (ModuleName.Contains(TEXT("Marketplace")) || ModuleName.Contains(TEXT("Public")))
	{
		return EStationModuleGroup::Public;
	}
	else if (ModuleName.Contains(TEXT("Corridor")) || ModuleName.Contains(TEXT("Connector")))
	{
		return EStationModuleGroup::Connection;
	}

	return EStationModuleGroup::Other;
}
