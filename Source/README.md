# Source Code Structure

This directory contains the C++ source code for the Modular Station Designer plugin.

## Module Overview

The plugin is organized into two Unreal Engine modules:

### ModularStationDesigner (Runtime Module)
Runtime functionality that can be used in both editor and game.

**Location:** `Source/ModularStationDesigner/`

**Key Classes:**
- `UConnectionPointComponent` - Component for module connection points with snapping
- `FStationDesignerTypes` - Core data structures (FStationDesign, FModulePlacement, enums)
- `FSnappingHelper` - Utilities for module snapping and alignment

**Features:**
- Connection point validation and compatibility checking
- Automatic snapping algorithms
- Grid and rotation snapping
- Module transform calculations

---

### ModularStationDesignerEditor (Editor Module)
Editor-only functionality for the Station Builder UI.

**Location:** `Source/ModularStationDesignerEditor/`

**Key Classes:**

#### UI Components (Slate Widgets)
- `SStationDesignerWindow` - Main plugin window with three-panel layout
- `SModulePalette` - Module browser with search and filtering
- `SPropertiesPanel` - Property editor for selected modules and station info

#### Core Systems
- `FModuleDiscovery` - Scans and loads Adastrea station modules from assets
- `FStationValidator` - Validates station designs (connectivity, power, requirements)
- `FStationExporter` - Exports station designs to Unreal Blueprints
- `FTemplateManager` - Manages station templates and presets

#### Utilities
- `FStationFileHelper` - File I/O for saving/loading station designs
- `FStationCommandManager` - Undo/redo system using command pattern
- `FAdvancedTools` - Copy/paste, mirror, rotate operations
- `FVisualizationSystem` - Power flow and connection visualization

---

## Code Organization

```
Source/
├── ModularStationDesigner/           # Runtime module
│   ├── Public/                       # Public headers (API)
│   │   ├── ModularStationDesigner.h
│   │   ├── StationDesignerTypes.h
│   │   ├── ConnectionPoint.h
│   │   └── SnappingHelper.h
│   ├── Private/                      # Implementation files
│   │   ├── ModularStationDesigner.cpp
│   │   ├── ConnectionPoint.cpp
│   │   └── SnappingHelper.cpp
│   └── ModularStationDesigner.Build.cs
│
└── ModularStationDesignerEditor/     # Editor module
    ├── Public/                       # Public headers
    │   ├── ModularStationDesignerEditor.h
    │   ├── StationDesignerWindow.h
    │   ├── ModulePalette.h
    │   ├── PropertiesPanel.h
    │   ├── ModuleDiscovery.h
    │   ├── StationValidator.h
    │   ├── StationExporter.h
    │   ├── StationFileHelper.h
    │   ├── StationCommandManager.h
    │   ├── TemplateManager.h
    │   ├── AdvancedTools.h
    │   └── VisualizationSystem.h
    ├── Private/                      # Implementation files
    │   ├── ModularStationDesignerEditor.cpp
    │   ├── StationDesignerWindow.cpp
    │   ├── ModulePalette.cpp
    │   ├── PropertiesPanel.cpp
    │   ├── ModuleDiscovery.cpp
    │   ├── StationValidator.cpp
    │   ├── StationExporter.cpp
    │   ├── StationFileHelper.cpp
    │   ├── StationCommandManager.cpp
    │   ├── TemplateManager.cpp
    │   ├── AdvancedTools.cpp
    │   └── VisualizationSystem.cpp
    └── ModularStationDesignerEditor.Build.cs
```

---

## Key Workflows

### Module Discovery
1. `FModuleDiscovery::DiscoverModules()` - Scans Asset Registry for Blueprints
2. Checks for `ASpaceStationModule` inheritance (Adastrea integration)
3. Extracts module metadata (type, group, power consumption)
4. Returns `TArray<FModuleInfo>` for UI display

### Station Validation
1. `FStationValidator::ValidateStation()` - Runs all validation checks
2. Checks: Required modules, connectivity (BFS), power balance, compatibility
3. Returns `TArray<FValidationMessage>` with severity levels
4. UI displays warnings/errors to user

### Save/Load
1. `FStationFileHelper::SaveStationToFile()` - Serializes to JSON
2. Saves to `<Project>/Saved/StationDesigns/*.station`
3. `FStationFileHelper::LoadStationFromFile()` - Deserializes from JSON
4. Supports backup creation and CSV export

### Undo/Redo
1. User action creates `IStationCommand` (Add, Remove, Move, Connect)
2. `FStationCommandManager::ExecuteCommand()` - Executes and stores in history
3. `Undo()` / `Redo()` - Reverses or re-applies commands
4. Maximum history size: 50 commands (configurable)

---

## Integration with Adastrea

The plugin has **optional integration** with the Adastrea game project:

**With Adastrea (`ADASTREA_INTEGRATION_ENABLED`):**
- Uses `ASpaceStationModule` for inheritance checking
- Reads actual module properties from CDO
- Uses `EStationModuleGroup` from Adastrea
- Exports to `ASpaceStation` Blueprints

**Without Adastrea (Standalone):**
- Falls back to name-based module discovery
- Infers properties from module names
- Uses local `EStationModuleGroup` enum
- Exports to generic `AActor` Blueprints

Detection is automatic via `__has_include` preprocessor check in `StationDesignerTypes.h`.

---

## Build Dependencies

### Runtime Module
- Core, CoreUObject, Engine, InputCore
- Json, JsonUtilities

### Editor Module
- All runtime dependencies
- Slate, SlateCore, UnrealEd, LevelEditor
- PropertyEditor, AssetTools, EditorStyle
- Kismet, KismetCompiler, BlueprintGraph
- ToolMenus, WorkspaceMenuStructure

See `*.Build.cs` files for complete dependency lists.

---

## Development Guidelines

### Code Style
- Follow Unreal Engine coding standards
- Use `U` prefix for UObject classes
- Use `F` prefix for structs/non-UObject classes
- Use `S` prefix for Slate widgets
- Use `E` prefix for enums

### Logging
- Use `UE_LOG(LogTemp, ...)` for debug messages
- Log levels: Verbose, Log, Warning, Error

### Memory Management
- Use `TSharedPtr`/`TSharedRef` for Slate widgets
- Use `TArray` for dynamic arrays
- Avoid raw pointers for UObjects (use `TWeakObjectPtr` if needed)

### Slate UI
- Use `SNew()` macro for widget creation
- Use lambda functions for dynamic text/visibility
- Cache widgets with `TSharedPtr` for updates

---

## Testing

**Note:** The plugin requires Unreal Engine to compile and test. Unit tests are not yet implemented.

**Manual Testing:**
1. Load plugin in Unreal Editor
2. Open Station Builder window (Tools menu)
3. Test module discovery (should list available modules)
4. Test UI interactions (palette, properties, toolbar)
5. Test save/load functionality
6. Test validation system

---

## Current Status

✅ **Complete:**
- Plugin structure and build configuration
- Core data structures and types
- Connection point system with validation
- Module discovery (with/without Adastrea)
- UI framework (window, palette, properties)
- Validation system with graph traversal
- Save/Load with JSON serialization
- Template system with starter templates
- Undo/Redo command system
- File management utilities
- Snapping algorithms

🚧 **In Progress:**
- 3D viewport integration
- Drag-and-drop functionality
- Real-time connection visualization
- Blueprint export implementation

📋 **Planned:**
- Advanced editing tools (copy/paste, mirror)
- Power flow visualization
- Multi-module selection
- Keyboard shortcuts
- Performance optimization

---

## Documentation

- [Architecture](../../docs/ARCHITECTURE.md) - Technical design
- [Roadmap](../../ROADMAP.md) - Development plan
- [User Guide](../../docs/USER_GUIDE.md) - Feature documentation
- [Setup](../../docs/SETUP.md) - Development environment

---

**Last Updated:** 2026-01-21  
**Plugin Version:** 0.1.0-alpha  
**Unreal Engine:** 5.6 (5.4+ supported)
