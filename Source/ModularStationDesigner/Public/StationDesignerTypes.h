// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Include Adastrea types if available
#if __has_include("Stations/StationModuleTypes.h")
#include "Stations/StationModuleTypes.h"
#define ADASTREA_INTEGRATION_ENABLED 1
#endif

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
 * NOTE: When building with Adastrea project, use EStationModuleGroup from Adastrea
 * This local definition is for standalone plugin development only
 */
#ifndef ADASTREA_INTEGRATION_ENABLED
UENUM(BlueprintType)
enum class EStationModuleGroup : uint8
{
	All          UMETA(DisplayName="All"),
	Docking      UMETA(DisplayName="Docking"),
	Power        UMETA(DisplayName="Power"),
	Storage      UMETA(DisplayName="Storage"),
	Processing   UMETA(DisplayName="Processing"),
	Defence      UMETA(DisplayName="Defence"),
	Habitation   UMETA(DisplayName="Habitation"),
	Public       UMETA(DisplayName="Public"),
	Connection   UMETA(DisplayName="Connection"),
	Other        UMETA(DisplayName="Other")
};
#endif // !ADASTREA_INTEGRATION_ENABLED

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
