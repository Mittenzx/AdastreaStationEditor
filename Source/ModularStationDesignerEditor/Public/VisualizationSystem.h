// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StationDesignerTypes.h"

/**
 * Visualization system for station design
 */
class FVisualizationSystem
{
public:
	// Power flow visualization
	struct FPowerFlowVisualization
	{
		FVector StartPoint;
		FVector EndPoint;
		float PowerAmount;
		FLinearColor Color;
		bool bIsGenerating; // true if generating, false if consuming
	};
	
	static TArray<FPowerFlowVisualization> GeneratePowerFlowLines(const FStationDesign& Design);
	
	// Module group color coding
	static FLinearColor GetColorForModuleGroup(EStationModuleGroup Group);
	static void SetCustomColorScheme(const TMap<EStationModuleGroup, FLinearColor>& ColorScheme);
	static TMap<EStationModuleGroup, FLinearColor> GetCurrentColorScheme();
	
	// Connection wire display
	struct FConnectionWire
	{
		FVector StartPoint;
		FVector EndPoint;
		EConnectionType ConnectionType;
		FLinearColor Color;
		bool bIsValid;
	};
	
	static TArray<FConnectionWire> GenerateConnectionWires(const FStationDesign& Design);
	
	// Traffic flow visualization (optional)
	struct FTrafficPath
	{
		TArray<FVector> PathPoints;
		FLinearColor Color;
		float Speed;
	};
	
	static TArray<FTrafficPath> GenerateTrafficPaths(const FStationDesign& Design);
	
	// Visualization settings
	struct FVisualizationSettings
	{
		bool bShowPowerFlow;
		bool bShowConnectionWires;
		bool bShowTrafficFlow;
		bool bUseModuleGroupColors;
		float PowerFlowAnimationSpeed;
		float WireThickness;
		
		FVisualizationSettings()
			: bShowPowerFlow(true)
			, bShowConnectionWires(true)
			, bShowTrafficFlow(false)
			, bUseModuleGroupColors(true)
			, PowerFlowAnimationSpeed(1.0f)
			, WireThickness(2.0f)
		{
		}
	};
	
	static FVisualizationSettings& GetSettings();

private:
	static TMap<EStationModuleGroup, FLinearColor> CustomColorScheme;
	static FVisualizationSettings Settings;
	
	// Default color scheme
	static TMap<EStationModuleGroup, FLinearColor> CreateDefaultColorScheme();
};
