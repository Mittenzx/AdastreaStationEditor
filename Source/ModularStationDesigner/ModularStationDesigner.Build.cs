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
			"InputCore",
			"Adastrea"  // Required compile-time dependency
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Json",
			"JsonUtilities"
		});
	}
}
