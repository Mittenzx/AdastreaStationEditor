// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Include Adastrea types - required for compilation
// This plugin requires the Adastrea module to be installed and available
// as it provides the core EStationModuleGroup enumeration
#include "Stations/StationModuleTypes.h"

#include "StationDesignerTypes.generated.h"

/**
 * Connection type enumeration for module connection points
 */
UENUM(BlueprintType)
enum class EConnectionType : uint8
{
	Standard     UMETA(DisplayName="Standard"),
	Docking      UMETA(DisplayName="Docking"),
	Corridor     UMETA(DisplayName="Corridor"),
	Power        UMETA(DisplayName="Power"),
	Universal    UMETA(DisplayName="Universal")
};

/**
 * Connection size enumeration for compatibility checking
 */
UENUM(BlueprintType)
enum class EConnectionSize : uint8
{
	Small        UMETA(DisplayName="Small"),
	Medium       UMETA(DisplayName="Medium"),
	Large        UMETA(DisplayName="Large"),
	Universal    UMETA(DisplayName="Universal")
};

/**
 * Station module group classification
 * 
 * NOTE: Uses EStationModuleGroup from Adastrea's StationModuleTypes.h
 * The enum is defined in the Adastrea module and must be included above.
 */

/**
 * Data structure representing a placed module in the station design
 */
USTRUCT(BlueprintType)
struct FModulePlacement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Module")
	FString ModuleID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Module")
	FSoftClassPath ModuleBlueprintPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Module")
	FTransform Transform;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Module")
	FString ComponentName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Module")
	TArray<FString> ConnectedModuleIDs;

	FModulePlacement()
		: ModuleID(TEXT(""))
		, ComponentName(TEXT(""))
	{
	}
};

/**
 * Data structure representing a complete station design
 */
USTRUCT(BlueprintType)
struct FStationDesign
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Station")
	FString StationName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Station")
	TArray<FModulePlacement> Modules;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Station")
	FString DesignVersion;

	FStationDesign()
		: StationName(TEXT("New Station"))
		, DesignVersion(TEXT("1.0"))
	{
	}
};
