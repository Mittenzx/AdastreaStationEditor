// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StationDesignerTypes.h"

/**
 * Advanced editing tools for station design
 */
class FAdvancedTools
{
public:
	// Copy/Paste functionality
	static void CopyModules(const TArray<FModulePlacement>& Modules);
	static TArray<FModulePlacement> PasteModules(const FVector& Offset);
	static bool HasCopiedModules();
	static void ClearClipboard();
	
	// Mirror functionality
	enum class EMirrorAxis : uint8
	{
		X,
		Y,
		Z
	};
	
	static TArray<FModulePlacement> MirrorModules(
		const TArray<FModulePlacement>& Modules,
		EMirrorAxis Axis,
		const FVector& MirrorPoint = FVector::ZeroVector
	);
	
	// Rotate functionality
	static TArray<FModulePlacement> RotateModules(
		const TArray<FModulePlacement>& Modules,
		float AngleDegrees,
		const FVector& RotationCenter = FVector::ZeroVector
	);
	
	// Symmetry mode
	static void EnableSymmetryMode(EMirrorAxis Axis);
	static void DisableSymmetryMode();
	static bool IsSymmetryModeEnabled();
	static EMirrorAxis GetSymmetryAxis();
	
	// Duplicate with offset
	static TArray<FModulePlacement> DuplicateModules(
		const TArray<FModulePlacement>& Modules,
		const FVector& Offset
	);
	
	// Auto-arrange helpers
	static void ArrangeInGrid(TArray<FModulePlacement>& Modules, float Spacing = 500.0f);
	static void ArrangeInCircle(TArray<FModulePlacement>& Modules, float Radius = 1000.0f);
	static void ArrangeInLine(TArray<FModulePlacement>& Modules, const FVector& Direction, float Spacing = 500.0f);

private:
	static TArray<FModulePlacement> ClipboardModules;
	static bool bSymmetryEnabled;
	static EMirrorAxis SymmetryAxis;
	
	// Helper to generate new module IDs
	static FString GenerateModuleID();
	static int32 NextModuleID;
};
