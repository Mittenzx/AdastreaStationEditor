// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationFileHelper.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "Json.h"
#include "JsonUtilities.h"

bool FStationFileHelper::SaveStationToFile(const FStationDesign& Design, const FString& FilePath, bool bPrettyPrint)
{
	// Ensure directory exists
	FString Directory = FPaths::GetPath(FilePath);
	EnsureDirectoryExists(Directory);
	
	// Convert to JSON
	FString JsonString;
	if (bPrettyPrint)
	{
		// Pretty print with indentation
		TSharedRef<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Design);
		if (JsonObject->Values.Num() > 0)
		{
			TSharedRef<TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>> JsonWriter = 
				TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>>::Create(&JsonString);
			FJsonSerializer::Serialize(JsonObject, JsonWriter);
		}
	}
	else
	{
		// Compact JSON
		if (!FJsonObjectConverter::UStructToJsonObjectString(Design, JsonString))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to serialize station design to JSON"));
			return false;
		}
	}
	
	// Save to file
	if (!FFileHelper::SaveStringToFile(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save station file: %s"), *FilePath);
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Station saved successfully: %s"), *FilePath);
	return true;
}

bool FStationFileHelper::LoadStationFromFile(const FString& FilePath, FStationDesign& OutDesign)
{
	// Check if file exists
	if (!FPaths::FileExists(FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Station file not found: %s"), *FilePath);
		return false;
	}
	
	// Load file contents
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load station file: %s"), *FilePath);
		return false;
	}
	
	// Parse JSON
	if (!FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &OutDesign, 0, 0))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse station JSON: %s"), *FilePath);
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Station loaded successfully: %s"), *FilePath);
	return true;
}

FString FStationFileHelper::GetStationDesignsDirectory()
{
	FString Directory = FPaths::ProjectSavedDir() / TEXT("StationDesigns");
	EnsureDirectoryExists(Directory);
	return Directory;
}

TArray<FString> FStationFileHelper::GetAvailableStationFiles()
{
	TArray<FString> StationFiles;
	FString Directory = GetStationDesignsDirectory();
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	// Find all .station files
	PlatformFile.FindFiles(StationFiles, *Directory, TEXT(".station"));
	
	// Sort by modification time (newest first)
	StationFiles.Sort([&PlatformFile](const FString& A, const FString& B)
	{
		return PlatformFile.GetTimeStamp(*A) > PlatformFile.GetTimeStamp(*B);
	});
	
	return StationFiles;
}

bool FStationFileHelper::IsValidStationFile(const FString& FilePath)
{
	if (!FPaths::FileExists(FilePath))
	{
		return false;
	}
	
	// Check file extension
	if (!FilePath.EndsWith(TEXT(".station")))
	{
		return false;
	}
	
	// Try to parse the file
	FStationDesign TestDesign;
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		return false;
	}
	
	return FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &TestDesign, 0, 0);
}

bool FStationFileHelper::GetStationFileInfo(const FString& FilePath, FString& OutName, int32& OutModuleCount, FDateTime& OutTimestamp)
{
	// Get file timestamp
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	OutTimestamp = PlatformFile.GetTimeStamp(*FilePath);
	
	// Load and parse file
	FStationDesign Design;
	if (!LoadStationFromFile(FilePath, Design))
	{
		return false;
	}
	
	OutName = Design.StationName;
	OutModuleCount = Design.Modules.Num();
	
	return true;
}

FString FStationFileHelper::BackupStationFile(const FString& FilePath)
{
	if (!FPaths::FileExists(FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot backup non-existent file: %s"), *FilePath);
		return FString();
	}
	
	// Generate backup filename with timestamp
	FString Directory = FPaths::GetPath(FilePath);
	FString Filename = FPaths::GetBaseFilename(FilePath);
	FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
	FString BackupPath = Directory / (Filename + TEXT("_backup_") + Timestamp + TEXT(".station"));
	
	// Copy file
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.CopyFile(*BackupPath, *FilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Backup created: %s"), *BackupPath);
		return BackupPath;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Failed to create backup of: %s"), *FilePath);
	return FString();
}

bool FStationFileHelper::DeleteStationFile(const FString& FilePath, bool bCreateBackup)
{
	if (!FPaths::FileExists(FilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot delete non-existent file: %s"), *FilePath);
		return false;
	}
	
	// Create backup if requested
	if (bCreateBackup)
	{
		FString BackupPath = BackupStationFile(FilePath);
		if (BackupPath.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create backup before deletion"));
		}
	}
	
	// Delete file
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (PlatformFile.DeleteFile(*FilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Station file deleted: %s"), *FilePath);
		return true;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Failed to delete station file: %s"), *FilePath);
	return false;
}

bool FStationFileHelper::ExportStationDesign(const FStationDesign& Design, const FString& FilePath, const FString& Format)
{
	if (Format.Equals(TEXT("json"), ESearchCase::IgnoreCase))
	{
		return SaveStationToFile(Design, FilePath, true);
	}
	else if (Format.Equals(TEXT("csv"), ESearchCase::IgnoreCase))
	{
		// Export to CSV format (module list)
		FString CsvContent = TEXT("Module ID,Component Name,Blueprint Path,Position X,Position Y,Position Z,Connections\n");
		
		for (const FModulePlacement& Module : Design.Modules)
		{
			FVector Location = Module.Transform.GetLocation();
			FString ConnectionsList = FString::Join(Module.ConnectedModuleIDs, TEXT(";"));
			
			CsvContent += FString::Printf(TEXT("%s,%s,%s,%.2f,%.2f,%.2f,\"%s\"\n"),
				*Module.ModuleID,
				*Module.ComponentName,
				*Module.ModuleBlueprintPath.GetAssetPathString(),
				Location.X, Location.Y, Location.Z,
				*ConnectionsList);
		}
		
		return FFileHelper::SaveStringToFile(CsvContent, *FilePath);
	}
	
	UE_LOG(LogTemp, Error, TEXT("Unsupported export format: %s"), *Format);
	return false;
}

void FStationFileHelper::EnsureDirectoryExists(const FString& Directory)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.DirectoryExists(*Directory))
	{
		PlatformFile.CreateDirectoryTree(*Directory);
		UE_LOG(LogTemp, Log, TEXT("Created directory: %s"), *Directory);
	}
}

FString FStationFileHelper::GenerateUniqueFilename(const FString& Directory, const FString& BaseName, const FString& Extension)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	FString Filename = Directory / BaseName + Extension;
	int32 Counter = 1;
	
	while (PlatformFile.FileExists(*Filename))
	{
		Filename = Directory / FString::Printf(TEXT("%s_%d%s"), *BaseName, Counter, *Extension);
		Counter++;
	}
	
	return Filename;
}
