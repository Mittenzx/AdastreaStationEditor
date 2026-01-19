// Copyright Epic Games, Inc. All Rights Reserved.

#include "VisualizationSystem.h"

// Static member initialization
TMap<EStationModuleGroup, FLinearColor> FVisualizationSystem::CustomColorScheme;
FVisualizationSystem::FVisualizationSettings FVisualizationSystem::Settings;

TArray<FVisualizationSystem::FPowerFlowVisualization> FVisualizationSystem::GeneratePowerFlowLines(const FStationDesign& Design)
{
	TArray<FPowerFlowVisualization> PowerFlows;
	
	// Find power generators and consumers
	TArray<FModulePlacement> Generators;
	TArray<FModulePlacement> Consumers;
	
	for (const FModulePlacement& Module : Design.Modules)
	{
		FString ModuleName = Module.ModuleBlueprintPath.GetAssetName();
		if (ModuleName.Contains(TEXT("Reactor")) || ModuleName.Contains(TEXT("Solar")))
		{
			Generators.Add(Module);
		}
		else
		{
			Consumers.Add(Module);
		}
	}
	
	// Create power flow lines from generators to consumers
	for (const FModulePlacement& Generator : Generators)
	{
		for (const FModulePlacement& Consumer : Consumers)
		{
			FPowerFlowVisualization Flow;
			Flow.StartPoint = Generator.Transform.GetLocation();
			Flow.EndPoint = Consumer.Transform.GetLocation();
			Flow.PowerAmount = 100.0f; // Simplified
			Flow.bIsGenerating = true;
			Flow.Color = FLinearColor::Green;
			
			PowerFlows.Add(Flow);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Generated %d power flow visualizations"), PowerFlows.Num());
	return PowerFlows;
}

FLinearColor FVisualizationSystem::GetColorForModuleGroup(EStationModuleGroup Group)
{
	if (CustomColorScheme.Num() == 0)
	{
		CustomColorScheme = CreateDefaultColorScheme();
	}
	
	if (CustomColorScheme.Contains(Group))
	{
		return CustomColorScheme[Group];
	}
	
	return FLinearColor::White;
}

void FVisualizationSystem::SetCustomColorScheme(const TMap<EStationModuleGroup, FLinearColor>& ColorScheme)
{
	CustomColorScheme = ColorScheme;
}

TMap<EStationModuleGroup, FLinearColor> FVisualizationSystem::GetCurrentColorScheme()
{
	if (CustomColorScheme.Num() == 0)
	{
		CustomColorScheme = CreateDefaultColorScheme();
	}
	
	return CustomColorScheme;
}

TArray<FVisualizationSystem::FConnectionWire> FVisualizationSystem::GenerateConnectionWires(const FStationDesign& Design)
{
	TArray<FConnectionWire> Wires;
	
	// Generate wires based on connections
	for (const FModulePlacement& Module : Design.Modules)
	{
		for (const FString& ConnectedID : Module.ConnectedModuleIDs)
		{
			// Find connected module
			const FModulePlacement* ConnectedModule = Design.Modules.FindByPredicate(
				[&ConnectedID](const FModulePlacement& M) { return M.ModuleID == ConnectedID; }
			);
			
			if (ConnectedModule)
			{
				FConnectionWire Wire;
				Wire.StartPoint = Module.Transform.GetLocation();
				Wire.EndPoint = ConnectedModule->Transform.GetLocation();
				Wire.ConnectionType = EConnectionType::Standard; // Simplified
				Wire.Color = FLinearColor::Blue;
				Wire.bIsValid = true;
				
				Wires.Add(Wire);
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Generated %d connection wires"), Wires.Num());
	return Wires;
}

TArray<FVisualizationSystem::FTrafficPath> FVisualizationSystem::GenerateTrafficPaths(const FStationDesign& Design)
{
	TArray<FTrafficPath> Paths;
	
	// Find docking modules
	TArray<FModulePlacement> DockingModules;
	for (const FModulePlacement& Module : Design.Modules)
	{
		FString ModuleName = Module.ModuleBlueprintPath.GetAssetName();
		if (ModuleName.Contains(TEXT("Docking")))
		{
			DockingModules.Add(Module);
		}
	}
	
	// Create traffic paths from docking to marketplace/storage
	for (const FModulePlacement& Docking : DockingModules)
	{
		for (const FModulePlacement& Module : Design.Modules)
		{
			FString ModuleName = Module.ModuleBlueprintPath.GetAssetName();
			if (ModuleName.Contains(TEXT("Marketplace")) || ModuleName.Contains(TEXT("Cargo")))
			{
				FTrafficPath Path;
				Path.PathPoints.Add(Docking.Transform.GetLocation());
				
				// Add intermediate point
				FVector MidPoint = (Docking.Transform.GetLocation() + Module.Transform.GetLocation()) * 0.5f;
				MidPoint.Z += 200.0f; // Arc above
				Path.PathPoints.Add(MidPoint);
				
				Path.PathPoints.Add(Module.Transform.GetLocation());
				Path.Color = FLinearColor::Yellow;
				Path.Speed = 100.0f;
				
				Paths.Add(Path);
			}
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("Generated %d traffic paths"), Paths.Num());
	return Paths;
}

FVisualizationSystem::FVisualizationSettings& FVisualizationSystem::GetSettings()
{
	return Settings;
}

TMap<EStationModuleGroup, FLinearColor> FVisualizationSystem::CreateDefaultColorScheme()
{
	TMap<EStationModuleGroup, FLinearColor> DefaultScheme;
	
	DefaultScheme.Add(EStationModuleGroup::All, FLinearColor::White);
	DefaultScheme.Add(EStationModuleGroup::Docking, FLinearColor(0.0f, 0.5f, 1.0f)); // Light Blue
	DefaultScheme.Add(EStationModuleGroup::Power, FLinearColor(1.0f, 1.0f, 0.0f)); // Yellow
	DefaultScheme.Add(EStationModuleGroup::Storage, FLinearColor(0.5f, 0.3f, 0.1f)); // Brown
	DefaultScheme.Add(EStationModuleGroup::Processing, FLinearColor(1.0f, 0.5f, 0.0f)); // Orange
	DefaultScheme.Add(EStationModuleGroup::Defence, FLinearColor(1.0f, 0.0f, 0.0f)); // Red
	DefaultScheme.Add(EStationModuleGroup::Habitation, FLinearColor(0.0f, 1.0f, 0.5f)); // Green
	DefaultScheme.Add(EStationModuleGroup::Public, FLinearColor(0.5f, 0.0f, 1.0f)); // Purple
	DefaultScheme.Add(EStationModuleGroup::Connection, FLinearColor(0.7f, 0.7f, 0.7f)); // Gray
	DefaultScheme.Add(EStationModuleGroup::Other, FLinearColor(0.5f, 0.5f, 0.5f)); // Dark Gray
	
	return DefaultScheme;
}
