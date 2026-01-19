// Copyright Epic Games, Inc. All Rights Reserved.

#include "StationValidator.h"

TArray<FValidationMessage> FStationValidator::ValidateStation(const FStationDesign& Design)
{
	TArray<FValidationMessage> Messages;

	// Run all validation checks
	CheckRequiredModules(Design, Messages);
	CheckConnectivity(Design, Messages);
	CheckPowerBalance(Design, Messages);
	CheckModuleCompatibility(Design, Messages);

	return Messages;
}

void FStationValidator::CheckRequiredModules(const FStationDesign& Design, TArray<FValidationMessage>& OutMessages)
{
	if (Design.Modules.Num() == 0)
	{
		OutMessages.Add(FValidationMessage(
			EValidationSeverity::Error,
			TEXT("Station has no modules. Add at least one docking module to begin."),
			TEXT("")
		));
		return;
	}

	// Check for at least one docking module
	bool bHasDocking = false;
	bool bHasMarketplace = false;
	bool bHasStorage = false;

	for (const FModulePlacement& Module : Design.Modules)
	{
		FString ModuleName = Module.ModuleBlueprintPath.GetAssetName();
		
		if (ModuleName.Contains(TEXT("Docking")))
		{
			bHasDocking = true;
		}
		if (ModuleName.Contains(TEXT("Marketplace")))
		{
			bHasMarketplace = true;
		}
		if (ModuleName.Contains(TEXT("Cargo")) || ModuleName.Contains(TEXT("Storage")))
		{
			bHasStorage = true;
		}
	}

	// Critical: Must have docking
	if (!bHasDocking)
	{
		OutMessages.Add(FValidationMessage(
			EValidationSeverity::Error,
			TEXT("Station must have at least one docking facility (DockingBay or DockingPort)."),
			TEXT("")
		));
	}

	// Warnings for recommended modules
	if (!bHasMarketplace)
	{
		OutMessages.Add(FValidationMessage(
			EValidationSeverity::Warning,
			TEXT("No marketplace found - station cannot engage in trading."),
			TEXT("")
		));
	}

	if (!bHasStorage)
	{
		OutMessages.Add(FValidationMessage(
			EValidationSeverity::Warning,
			TEXT("No cargo storage - station has limited inventory capacity."),
			TEXT("")
		));
	}
}

void FStationValidator::CheckConnectivity(const FStationDesign& Design, TArray<FValidationMessage>& OutMessages)
{
	if (Design.Modules.Num() <= 1)
	{
		return; // Single module or empty station doesn't need connectivity check
	}

	// Check if all modules are connected (simplified check)
	// In a full implementation, this would use graph traversal
	for (const FModulePlacement& Module : Design.Modules)
	{
		if (Module.ConnectedModuleIDs.Num() == 0 && Design.Modules.Num() > 1)
		{
			OutMessages.Add(FValidationMessage(
				EValidationSeverity::Warning,
				FString::Printf(TEXT("Module '%s' is not connected to other modules."), *Module.ComponentName),
				Module.ModuleID
			));
		}
	}
}

void FStationValidator::CheckPowerBalance(const FStationDesign& Design, TArray<FValidationMessage>& OutMessages)
{
	// Simplified power calculation
	// In a full implementation, this would query actual module power values
	float TotalPowerConsumption = Design.Modules.Num() * 50.0f; // Assume 50 MW per module
	float TotalPowerGeneration = 0.0f;

	// Count power generation modules
	for (const FModulePlacement& Module : Design.Modules)
	{
		FString ModuleName = Module.ModuleBlueprintPath.GetAssetName();
		if (ModuleName.Contains(TEXT("Reactor")) || ModuleName.Contains(TEXT("Solar")))
		{
			TotalPowerGeneration += 500.0f; // Reactor generates 500 MW
		}
	}

	float PowerDeficit = TotalPowerConsumption - TotalPowerGeneration;
	
	if (PowerDeficit > 0)
	{
		OutMessages.Add(FValidationMessage(
			EValidationSeverity::Warning,
			FString::Printf(TEXT("Power deficit: %.0f MW. Consider adding reactor or solar array modules."), PowerDeficit),
			TEXT("")
		));
	}
	else
	{
		OutMessages.Add(FValidationMessage(
			EValidationSeverity::Info,
			FString::Printf(TEXT("Power surplus: %.0f MW. Station has sufficient power."), -PowerDeficit),
			TEXT("")
		));
	}
}

void FStationValidator::CheckModuleCompatibility(const FStationDesign& Design, TArray<FValidationMessage>& OutMessages)
{
	// Check for basic compatibility issues
	// In a full implementation, this would check connection point compatibility
	
	if (Design.Modules.Num() > 50)
	{
		OutMessages.Add(FValidationMessage(
			EValidationSeverity::Warning,
			FString::Printf(TEXT("Station has %d modules. Performance may be impacted with more than 50 modules."), Design.Modules.Num()),
			TEXT("")
		));
	}
}
