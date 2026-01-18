// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdvancedTools.h"
#include "Kismet/KismetMathLibrary.h"

// Static member initialization
TArray<FModulePlacement> FAdvancedTools::ClipboardModules;
bool FAdvancedTools::bSymmetryEnabled = false;
FAdvancedTools::EMirrorAxis FAdvancedTools::SymmetryAxis = EMirrorAxis::X;
int32 FAdvancedTools::NextModuleID = 1000;

void FAdvancedTools::CopyModules(const TArray<FModulePlacement>& Modules)
{
	ClipboardModules = Modules;
	UE_LOG(LogTemp, Log, TEXT("Copied %d modules to clipboard"), Modules.Num());
}

TArray<FModulePlacement> FAdvancedTools::PasteModules(const FVector& Offset)
{
	TArray<FModulePlacement> PastedModules;
	
	for (const FModulePlacement& Module : ClipboardModules)
	{
		FModulePlacement NewModule = Module;
		NewModule.ModuleID = GenerateModuleID();
		NewModule.Transform.AddToTranslation(Offset);
		NewModule.ConnectedModuleIDs.Empty(); // Clear connections, will need to be re-established
		
		PastedModules.Add(NewModule);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Pasted %d modules with offset"), PastedModules.Num());
	return PastedModules;
}

bool FAdvancedTools::HasCopiedModules()
{
	return ClipboardModules.Num() > 0;
}

void FAdvancedTools::ClearClipboard()
{
	ClipboardModules.Empty();
}

TArray<FModulePlacement> FAdvancedTools::MirrorModules(
	const TArray<FModulePlacement>& Modules,
	EMirrorAxis Axis,
	const FVector& MirrorPoint)
{
	TArray<FModulePlacement> MirroredModules;
	
	for (const FModulePlacement& Module : Modules)
	{
		FModulePlacement NewModule = Module;
		NewModule.ModuleID = GenerateModuleID();
		
		FVector Position = Module.Transform.GetLocation();
		FVector MirroredPosition = Position;
		
		switch (Axis)
		{
		case EMirrorAxis::X:
			MirroredPosition.X = 2.0f * MirrorPoint.X - Position.X;
			break;
		case EMirrorAxis::Y:
			MirroredPosition.Y = 2.0f * MirrorPoint.Y - Position.Y;
			break;
		case EMirrorAxis::Z:
			MirroredPosition.Z = 2.0f * MirrorPoint.Z - Position.Z;
			break;
		}
		
		NewModule.Transform.SetLocation(MirroredPosition);
		
		// Mirror rotation
		FRotator Rotation = Module.Transform.GetRotation().Rotator();
		if (Axis == EMirrorAxis::X)
		{
			Rotation.Yaw = -Rotation.Yaw;
			Rotation.Roll = -Rotation.Roll;
		}
		else if (Axis == EMirrorAxis::Y)
		{
			Rotation.Pitch = -Rotation.Pitch;
			Rotation.Roll = -Rotation.Roll;
		}
		else if (Axis == EMirrorAxis::Z)
		{
			Rotation.Pitch = -Rotation.Pitch;
			Rotation.Yaw = -Rotation.Yaw;
		}
		NewModule.Transform.SetRotation(FQuat(Rotation));
		
		NewModule.ConnectedModuleIDs.Empty();
		MirroredModules.Add(NewModule);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Mirrored %d modules across axis %d"), MirroredModules.Num(), (int32)Axis);
	return MirroredModules;
}

TArray<FModulePlacement> FAdvancedTools::RotateModules(
	const TArray<FModulePlacement>& Modules,
	float AngleDegrees,
	const FVector& RotationCenter)
{
	TArray<FModulePlacement> RotatedModules;
	
	FQuat RotationQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(AngleDegrees));
	
	for (const FModulePlacement& Module : Modules)
	{
		FModulePlacement NewModule = Module;
		NewModule.ModuleID = GenerateModuleID();
		
		// Rotate position around center
		FVector Position = Module.Transform.GetLocation();
		FVector RelativePos = Position - RotationCenter;
		FVector RotatedPos = RotationQuat.RotateVector(RelativePos);
		NewModule.Transform.SetLocation(RotationCenter + RotatedPos);
		
		// Rotate orientation
		FQuat CurrentRotation = Module.Transform.GetRotation();
		FQuat NewRotation = RotationQuat * CurrentRotation;
		NewModule.Transform.SetRotation(NewRotation);
		
		NewModule.ConnectedModuleIDs.Empty();
		RotatedModules.Add(NewModule);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Rotated %d modules by %.1f degrees"), RotatedModules.Num(), AngleDegrees);
	return RotatedModules;
}

void FAdvancedTools::EnableSymmetryMode(EMirrorAxis Axis)
{
	bSymmetryEnabled = true;
	SymmetryAxis = Axis;
	UE_LOG(LogTemp, Log, TEXT("Symmetry mode enabled on axis %d"), (int32)Axis);
}

void FAdvancedTools::DisableSymmetryMode()
{
	bSymmetryEnabled = false;
	UE_LOG(LogTemp, Log, TEXT("Symmetry mode disabled"));
}

bool FAdvancedTools::IsSymmetryModeEnabled()
{
	return bSymmetryEnabled;
}

FAdvancedTools::EMirrorAxis FAdvancedTools::GetSymmetryAxis()
{
	return SymmetryAxis;
}

TArray<FModulePlacement> FAdvancedTools::DuplicateModules(
	const TArray<FModulePlacement>& Modules,
	const FVector& Offset)
{
	TArray<FModulePlacement> DuplicatedModules;
	
	for (const FModulePlacement& Module : Modules)
	{
		FModulePlacement NewModule = Module;
		NewModule.ModuleID = GenerateModuleID();
		NewModule.Transform.AddToTranslation(Offset);
		NewModule.ConnectedModuleIDs.Empty();
		
		DuplicatedModules.Add(NewModule);
	}
	
	return DuplicatedModules;
}

void FAdvancedTools::ArrangeInGrid(TArray<FModulePlacement>& Modules, float Spacing)
{
	int32 GridSize = FMath::CeilToInt(FMath::Sqrt(static_cast<float>(Modules.Num())));
	
	for (int32 i = 0; i < Modules.Num(); i++)
	{
		int32 Row = i / GridSize;
		int32 Col = i % GridSize;
		
		FVector NewPosition(Col * Spacing, Row * Spacing, 0.0f);
		Modules[i].Transform.SetLocation(NewPosition);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Arranged %d modules in %dx%d grid"), Modules.Num(), GridSize, GridSize);
}

void FAdvancedTools::ArrangeInCircle(TArray<FModulePlacement>& Modules, float Radius)
{
	if (Modules.Num() == 0) return;
	
	float AngleStep = 360.0f / Modules.Num();
	
	for (int32 i = 0; i < Modules.Num(); i++)
	{
		float Angle = FMath::DegreesToRadians(i * AngleStep);
		FVector NewPosition(
			FMath::Cos(Angle) * Radius,
			FMath::Sin(Angle) * Radius,
			0.0f
		);
		
		Modules[i].Transform.SetLocation(NewPosition);
		
		// Orient towards center
		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(NewPosition, FVector::ZeroVector);
		Modules[i].Transform.SetRotation(FQuat(LookRotation));
	}
	
	UE_LOG(LogTemp, Log, TEXT("Arranged %d modules in circle with radius %.1f"), Modules.Num(), Radius);
}

void FAdvancedTools::ArrangeInLine(TArray<FModulePlacement>& Modules, const FVector& Direction, float Spacing)
{
	FVector NormalizedDir = Direction.GetSafeNormal();
	
	for (int32 i = 0; i < Modules.Num(); i++)
	{
		FVector NewPosition = NormalizedDir * (i * Spacing);
		Modules[i].Transform.SetLocation(NewPosition);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Arranged %d modules in line"), Modules.Num());
}

FString FAdvancedTools::GenerateModuleID()
{
	return FString::Printf(TEXT("module_%04d"), NextModuleID++);
}
