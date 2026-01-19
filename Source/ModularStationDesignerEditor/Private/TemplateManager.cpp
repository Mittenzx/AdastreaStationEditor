// Copyright Epic Games, Inc. All Rights Reserved.

#include "TemplateManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"
#include "Json.h"
#include "JsonUtilities.h"

FString FTemplateManager::GetTemplatesDirectory()
{
	return FPaths::ProjectSavedDir() / TEXT("StationTemplates");
}

TArray<FStationDesign> FTemplateManager::LoadTemplates()
{
	TArray<FStationDesign> Templates;
	
	FString TemplatesDir = GetTemplatesDirectory();
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	if (!PlatformFile.DirectoryExists(*TemplatesDir))
	{
		PlatformFile.CreateDirectory(*TemplatesDir);
		CreateStarterTemplates();
	}
	
	TArray<FString> TemplateFiles;
	PlatformFile.FindFiles(TemplateFiles, *TemplatesDir, TEXT(".template"));
	
	for (const FString& FilePath : TemplateFiles)
	{
		FStationDesign Design;
		FString JsonString;
		if (FFileHelper::LoadFileToString(JsonString, *FilePath))
		{
			if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &Design, 0, 0))
			{
				Templates.Add(Design);
			}
		}
	}
	
	return Templates;
}

bool FTemplateManager::SaveAsTemplate(const FStationDesign& Design, const FString& TemplateName, const FString& Description)
{
	FString TemplatesDir = GetTemplatesDirectory();
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	if (!PlatformFile.DirectoryExists(*TemplatesDir))
	{
		PlatformFile.CreateDirectory(*TemplatesDir);
	}
	
	FString FilePath = TemplatesDir / TemplateName + TEXT(".template");
	
	FString JsonString;
	if (FJsonObjectConverter::UStructToJsonObjectString(Design, JsonString))
	{
		return FFileHelper::SaveStringToFile(JsonString, *FilePath);
	}
	
	return false;
}

bool FTemplateManager::LoadTemplate(const FString& TemplateName, FStationDesign& OutDesign)
{
	FString FilePath = GetTemplatesDirectory() / TemplateName + TEXT(".template");
	
	FString JsonString;
	if (FFileHelper::LoadFileToString(JsonString, *FilePath))
	{
		return FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &OutDesign, 0, 0);
	}
	
	return false;
}

TArray<FTemplateManager::FTemplateInfo> FTemplateManager::GetTemplateList()
{
	TArray<FTemplateInfo> TemplateList;
	
	FString TemplatesDir = GetTemplatesDirectory();
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	TArray<FString> TemplateFiles;
	PlatformFile.FindFiles(TemplateFiles, *TemplatesDir, TEXT(".template"));
	
	for (const FString& FilePath : TemplateFiles)
	{
		FTemplateInfo Info;
		Info.Name = FPaths::GetBaseFilename(FilePath);
		Info.FilePath = FilePath;
		Info.CreatedDate = PlatformFile.GetTimeStamp(*FilePath);
		
		// Load design to get module count
		FStationDesign Design;
		FString JsonString;
		if (FFileHelper::LoadFileToString(JsonString, *FilePath))
		{
			if (FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &Design, 0, 0))
			{
				Info.ModuleCount = Design.Modules.Num();
				Info.Description = FString::Printf(TEXT("%d modules"), Info.ModuleCount);
			}
		}
		
		TemplateList.Add(Info);
	}
	
	return TemplateList;
}

void FTemplateManager::CreateStarterTemplates()
{
	SaveAsTemplate(CreateTradeOutpostTemplate(), TEXT("TradeOutpost"), TEXT("Small trading station with basic facilities"));
	SaveAsTemplate(CreateMiningStationTemplate(), TEXT("MiningStation"), TEXT("Mining operation with storage and processing"));
	SaveAsTemplate(CreateResearchFacilityTemplate(), TEXT("ResearchFacility"), TEXT("Science station with laboratories"));
	SaveAsTemplate(CreateMilitaryBaseTemplate(), TEXT("MilitaryBase"), TEXT("Fortified military installation"));
	SaveAsTemplate(CreateTradingHubTemplate(), TEXT("TradingHub"), TEXT("Large commercial trading hub"));
}

FStationDesign FTemplateManager::CreateTradeOutpostTemplate()
{
	FStationDesign Design;
	Design.StationName = TEXT("Trade Outpost");
	Design.DesignVersion = TEXT("1.0");
	
	// Add basic modules
	FModulePlacement Docking;
	Docking.ModuleID = TEXT("module_001");
	Docking.ComponentName = TEXT("DockingPort");
	Docking.Transform = FTransform(FVector(0, 0, 0));
	Design.Modules.Add(Docking);
	
	FModulePlacement Marketplace;
	Marketplace.ModuleID = TEXT("module_002");
	Marketplace.ComponentName = TEXT("Marketplace");
	Marketplace.Transform = FTransform(FVector(500, 0, 0));
	Design.Modules.Add(Marketplace);
	
	FModulePlacement Cargo;
	Cargo.ModuleID = TEXT("module_003");
	Cargo.ComponentName = TEXT("CargoStorage");
	Cargo.Transform = FTransform(FVector(1000, 0, 0));
	Design.Modules.Add(Cargo);
	
	return Design;
}

FStationDesign FTemplateManager::CreateMiningStationTemplate()
{
	FStationDesign Design;
	Design.StationName = TEXT("Mining Station");
	Design.DesignVersion = TEXT("1.0");
	
	// Add mining-specific modules
	for (int32 i = 0; i < 5; i++)
	{
		FModulePlacement Module;
		Module.ModuleID = FString::Printf(TEXT("module_%03d"), i + 1);
		Module.ComponentName = (i == 0) ? TEXT("DockingBay") : 
		                       (i < 3) ? TEXT("CargoStorage") : TEXT("Processing");
		Module.Transform = FTransform(FVector(i * 500.0f, 0, 0));
		Design.Modules.Add(Module);
	}
	
	return Design;
}

FStationDesign FTemplateManager::CreateResearchFacilityTemplate()
{
	FStationDesign Design;
	Design.StationName = TEXT("Research Facility");
	Design.DesignVersion = TEXT("1.0");
	
	// Add research modules
	for (int32 i = 0; i < 4; i++)
	{
		FModulePlacement Module;
		Module.ModuleID = FString::Printf(TEXT("module_%03d"), i + 1);
		Module.ComponentName = (i == 0) ? TEXT("DockingPort") : TEXT("ScienceLab");
		Module.Transform = FTransform(FVector(i * 400.0f, 0, 0));
		Design.Modules.Add(Module);
	}
	
	return Design;
}

FStationDesign FTemplateManager::CreateMilitaryBaseTemplate()
{
	FStationDesign Design;
	Design.StationName = TEXT("Military Base");
	Design.DesignVersion = TEXT("1.0");
	
	// Add military modules
	for (int32 i = 0; i < 6; i++)
	{
		FModulePlacement Module;
		Module.ModuleID = FString::Printf(TEXT("module_%03d"), i + 1);
		Module.ComponentName = (i == 0) ? TEXT("DockingBay") :
		                       (i < 3) ? TEXT("Turret") :
		                       (i == 3) ? TEXT("ShieldGenerator") : TEXT("Barracks");
		Module.Transform = FTransform(FVector((i % 3) * 500.0f, (i / 3) * 500.0f, 0));
		Design.Modules.Add(Module);
	}
	
	return Design;
}

FStationDesign FTemplateManager::CreateTradingHubTemplate()
{
	FStationDesign Design;
	Design.StationName = TEXT("Trading Hub");
	Design.DesignVersion = TEXT("1.0");
	
	// Add large trading hub modules
	for (int32 i = 0; i < 9; i++)
	{
		FModulePlacement Module;
		Module.ModuleID = FString::Printf(TEXT("module_%03d"), i + 1);
		
		if (i < 2)
			Module.ComponentName = TEXT("DockingBay");
		else if (i < 4)
			Module.ComponentName = TEXT("Marketplace");
		else if (i < 7)
			Module.ComponentName = TEXT("CargoStorage");
		else
			Module.ComponentName = TEXT("Habitation");
			
		Module.Transform = FTransform(FVector((i % 3) * 600.0f, (i / 3) * 600.0f, 0));
		Design.Modules.Add(Module);
	}
	
	return Design;
}
