// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ModularStationDesigner : ModuleRules
{
	public ModularStationDesigner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Json",
			"JsonUtilities"
		});

		// Optional dependency on Adastrea module for enhanced integration
		// The plugin will detect Adastrea at compile time via __has_include
		// and enable advanced features when available
		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
			"Adastrea"
		});
	}
}
