// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ModularStationDesignerEditor : ModuleRules
{
	public ModularStationDesignerEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"ModularStationDesigner"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UnrealEd",
			"LevelEditor",
			"PropertyEditor",
			"AssetTools",
			"EditorStyle",
			"EditorWidgets",
			"InputCore",
			"Projects",
			"ToolMenus",
			"WorkspaceMenuStructure",
			"Json",
			"JsonUtilities"
		});
	}
}
