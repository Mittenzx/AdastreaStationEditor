// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StationDesignerTypes.h"

/**
 * File I/O utilities for station designs
 * Handles serialization, deserialization, and file management
 */
class FStationFileHelper
{
public:
	/**
	 * Save a station design to a JSON file
	 * @param Design The station design to save
	 * @param FilePath Full path to save file
	 * @param bPrettyPrint Whether to format JSON for readability
	 * @return True if save successful
	 */
	static bool SaveStationToFile(const FStationDesign& Design, const FString& FilePath, bool bPrettyPrint = true);
	
	/**
	 * Load a station design from a JSON file
	 * @param FilePath Full path to load file
	 * @param OutDesign The loaded station design
	 * @return True if load successful
	 */
	static bool LoadStationFromFile(const FString& FilePath, FStationDesign& OutDesign);
	
	/**
	 * Get the default save directory for station designs
	 * @return Path to save directory (creates if doesn't exist)
	 */
	static FString GetStationDesignsDirectory();
	
	/**
	 * Get list of available station design files
	 * @return Array of file paths
	 */
	static TArray<FString> GetAvailableStationFiles();
	
	/**
	 * Check if a file is a valid station design file
	 * @param FilePath Path to check
	 * @return True if valid station file
	 */
	static bool IsValidStationFile(const FString& FilePath);
	
	/**
	 * Get file info for a station design
	 * @param FilePath Path to station file
	 * @param OutName Station name from file
	 * @param OutModuleCount Number of modules
	 * @param OutTimestamp File modification time
	 * @return True if info retrieved successfully
	 */
	static bool GetStationFileInfo(const FString& FilePath, FString& OutName, int32& OutModuleCount, FDateTime& OutTimestamp);
	
	/**
	 * Create a backup of a station file
	 * @param FilePath Original file path
	 * @return Path to backup file, or empty string on failure
	 */
	static FString BackupStationFile(const FString& FilePath);
	
	/**
	 * Delete a station design file
	 * @param FilePath File to delete
	 * @param bCreateBackup Whether to create a backup before deletion
	 * @return True if deletion successful
	 */
	static bool DeleteStationFile(const FString& FilePath, bool bCreateBackup = true);
	
	/**
	 * Export station design to a different format (for debugging/inspection)
	 * @param Design Station design to export
	 * @param FilePath Output file path
	 * @param Format Export format ("json", "xml", "csv")
	 * @return True if export successful
	 */
	static bool ExportStationDesign(const FStationDesign& Design, const FString& FilePath, const FString& Format = TEXT("json"));
	
private:
	/**
	 * Ensure the designs directory exists
	 */
	static void EnsureDirectoryExists(const FString& Directory);
	
	/**
	 * Generate a unique filename
	 */
	static FString GenerateUniqueFilename(const FString& Directory, const FString& BaseName, const FString& Extension);
};
