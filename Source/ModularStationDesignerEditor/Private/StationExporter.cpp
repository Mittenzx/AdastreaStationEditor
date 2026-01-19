// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationExporter.h"
#include "Engine/Blueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "PackageTools.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Misc/FileHelper.h"

#if ADASTREA_INTEGRATION_ENABLED
#include "Stations/SpaceStation.h"
#endif

bool FStationExporter::ExportToBlueprintAsset(
	const FStationDesign& Design,
	const FString& TargetPackagePath,
	const FString& AssetName,
	FString& OutErrorMessage)
{
	if (Design.Modules.Num() == 0)
	{
		OutErrorMessage = TEXT("Cannot export empty station design.");
		return false;
	}

	// Create Blueprint asset
	UBlueprint* NewBlueprint = CreateBlueprintAsset(TargetPackagePath, AssetName);
	if (!NewBlueprint)
	{
		OutErrorMessage = TEXT("Failed to create Blueprint asset.");
		return false;
	}

	// Add Child Actor Components for each module
	AddChildActorComponents(NewBlueprint, Design);

	// Configure station defaults
	ConfigureStationDefaults(NewBlueprint, Design);

	// Compile and save
	FKismetEditorUtilities::CompileBlueprint(NewBlueprint);

	OutErrorMessage = TEXT("");
	return true;
}

bool FStationExporter::SaveDesignToFile(const FStationDesign& Design, const FString& FilePath)
{
	// Convert design to JSON
	FString JsonString;
	if (!FJsonObjectConverter::UStructToJsonObjectString(Design, JsonString))
	{
		return false;
	}

	// Write to file
	return FFileHelper::SaveStringToFile(JsonString, *FilePath);
}

bool FStationExporter::LoadDesignFromFile(const FString& FilePath, FStationDesign& OutDesign)
{
	// Read from file
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		return false;
	}

	// Parse JSON
	return FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &OutDesign, 0, 0);
}

UBlueprint* FStationExporter::CreateBlueprintAsset(const FString& PackagePath, const FString& AssetName)
{
	// Create package
	FString PackageName = PackagePath + TEXT("/") + AssetName;
	UPackage* Package = CreatePackage(*PackageName);
	if (!Package)
	{
		return nullptr;
	}

	// Determine parent class for the Blueprint
	UClass* ParentClass = AActor::StaticClass();
	
#if ADASTREA_INTEGRATION_ENABLED
	// When Adastrea is available, create Blueprint from ASpaceStation
	ParentClass = ASpaceStation::StaticClass();
	UE_LOG(LogTemp, Log, TEXT("Creating station Blueprint from ASpaceStation class"));
#else
	// Fallback: Create from AActor
	UE_LOG(LogTemp, Warning, TEXT("Adastrea not available - creating station Blueprint from AActor (limited functionality)"));
#endif

	// Create Blueprint
	UBlueprint* NewBlueprint = FKismetEditorUtilities::CreateBlueprint(
		ParentClass,
		Package,
		*AssetName,
		BPTYPE_Normal,
		UBlueprint::StaticClass(),
		UBlueprintGeneratedClass::StaticClass(),
		NAME_None
	);

	if (NewBlueprint)
	{
		// Mark package as dirty
		Package->MarkPackageDirty();
		
		// Notify asset registry
		FAssetRegistryModule::AssetCreated(NewBlueprint);
	}

	return NewBlueprint;
}

void FStationExporter::AddChildActorComponents(UBlueprint* Blueprint, const FStationDesign& Design)
{
	if (!Blueprint)
	{
		return;
	}

	// Note: In a full implementation, this would create Child Actor Components
	// for each module in the design and set their transforms
	// This requires more complex Blueprint manipulation APIs
	
	// Placeholder implementation
	UE_LOG(LogTemp, Log, TEXT("Adding %d module components to Blueprint"), Design.Modules.Num());
}

void FStationExporter::ConfigureStationDefaults(UBlueprint* Blueprint, const FStationDesign& Design)
{
	if (!Blueprint)
	{
		return;
	}

	// Note: In a full implementation, this would set station properties
	// like name, max structural integrity, etc.
	
	UE_LOG(LogTemp, Log, TEXT("Configuring station: %s"), *Design.StationName);
}
