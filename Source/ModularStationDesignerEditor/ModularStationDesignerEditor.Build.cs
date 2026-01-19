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
		// The plugin detects Adastrea at compile time via __has_include
		// When available: inheritance-based discovery, ASpaceStation Blueprints
		// When absent: name-based discovery, AActor Blueprints (fallback mode)
		DynamicallyLoadedModuleNames.AddRange(new string[]
		{
			"Adastrea"
		});
	}
}
