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

		// Optional dependency on Adastrea module for runtime compatibility
		// If Adastrea is present, we can use its classes directly
		// If not, the plugin can still compile but won't have full integration
		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.Add("Adastrea");
		}
	}
}
