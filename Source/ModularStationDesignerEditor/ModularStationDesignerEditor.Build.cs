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
			"JsonUtilities",
			"Kismet",
			"KismetCompiler",
			"BlueprintGraph"
		});

		// Optional dependency on Adastrea module for full integration
		// If Adastrea is present, we can discover ASpaceStationModule classes
		// and export to ASpaceStation Blueprints properly
		// If not, the plugin uses fallback behavior
		PrivateDependencyModuleNames.Add("Adastrea");
	}
}
