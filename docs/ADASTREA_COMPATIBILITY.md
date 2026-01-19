# Adastrea Compatibility Guide

**Version**: 1.0  
**Last Updated**: 2026-01-19  
**Status**: Integrated ✅

---

## Overview

The Modular Station Designer plugin is now **fully compatible** with the Adastrea game project's module system. This document explains the integration points and how the plugin works with Adastrea.

---

## Integration Architecture

### Module System Compatibility

The plugin now properly integrates with Adastrea's module system:

1. **Base Classes**
   - Adastrea modules inherit from: `ASpaceStationModule`
   - Adastrea stations inherit from: `ASpaceStation`
   - Plugin now creates Blueprints using these base classes

2. **Module Discovery**
   - ✅ Discovers modules by checking `ASpaceStationModule` inheritance
   - ✅ Reads module properties from Blueprint CDO (Class Default Object)
   - ✅ Falls back to name-based discovery if Adastrea not available

3. **Module Properties**
   - `ModuleType`: Read from Blueprint's `ASpaceStationModule::ModuleType`
   - `ModulePower`: Read from Blueprint's `ASpaceStationModule::ModulePower`
   - `ModuleGroup`: Read from Blueprint's `ASpaceStationModule::ModuleGroup`

4. **Export Format**
   - ✅ Generates `ASpaceStation` Blueprints (not generic `AActor`)
   - ✅ Uses Child Actor Components for each module
   - ✅ Compatible with Adastrea's station loading system

---

## Module Groups

The plugin uses Adastrea's `EStationModuleGroup` enum when integrated:

| Group | Purpose | Example Modules |
|-------|---------|-----------------|
| **Docking** | Ship docking facilities | Docking Bay, Docking Port |
| **Power** | Energy generation | Reactor, Solar Array |
| **Storage** | Cargo storage | Cargo Bay, Fuel Depot |
| **Processing** | Manufacturing | Fabrication, Science Lab |
| **Defence** | Weapons & shields | Turret, Shield Generator |
| **Habitation** | Crew quarters | Living Quarters |
| **Public** | Markets & trading | Marketplace |
| **Connection** | Corridors | Corridor modules |
| **Other** | Miscellaneous | Custom modules |

---

## Build Configuration

### Dynamic Loading Architecture

The plugin uses **dynamic module loading** to support optional Adastrea integration without hard dependencies:

```cs
// ModularStationDesignerEditor.Build.cs
DynamicallyLoadedModuleNames.AddRange(new string[]
{
    "Adastrea"  // Optional - loaded at runtime if available
});
```

### With Adastrea Integration (Recommended)

When Adastrea headers are available at compile time (detected via `__has_include()`):

**Enabled Features:**
- ✅ Inheritance-based module discovery via `ASpaceStationModule`
- ✅ Actual property reading from Blueprint CDOs
- ✅ `ASpaceStation` Blueprint generation
- ✅ Full module metadata (type, power, group)
- ✅ Uses Adastrea's `EStationModuleGroup` enum
- ✅ `ADASTREA_INTEGRATION_ENABLED` compile flag set

### Standalone Mode (Fallback)

When the plugin is built without Adastrea headers present:

**Fallback Behavior:**
- ⚠️ Name-based module discovery (pattern matching)
- ⚠️ Inferred module properties from names
- ⚠️ `AActor` Blueprint generation (limited functionality)
- ⚠️ Uses local `EStationModuleGroup` enum copy
- ⚠️ `ADASTREA_INTEGRATION_ENABLED` not defined

**Note**: The plugin will compile successfully in either mode. Conditional compilation ensures code references to Adastrea types are only active when headers are available.

---

## Module Discovery Logic

### With Adastrea Integration

```cpp
// Check if Blueprint inherits from ASpaceStationModule
if (Blueprint->GeneratedClass->IsChildOf(ASpaceStationModule::StaticClass()))
{
    // Read properties from CDO
    ASpaceStationModule* ModuleCDO = Blueprint->GeneratedClass->GetDefaultObject();
    Info.ModuleType = ModuleCDO->GetModuleType();
    Info.PowerConsumption = ModuleCDO->GetModulePower();
    Info.ModuleGroup = ModuleCDO->GetModuleGroup();
}
```

### Fallback (Standalone)

```cpp
// Name-based filtering
if (AssetName.Contains("Module") || AssetName.Contains("Station"))
{
    // Infer properties from name
    if (AssetName.Contains("Reactor"))
        Info.PowerConsumption = -500.0f;
}
```

---

## Blueprint Export

### With Adastrea Integration

```cpp
// Creates ASpaceStation Blueprint
UBlueprint* NewBlueprint = FKismetEditorUtilities::CreateBlueprint(
    ASpaceStation::StaticClass(),  // ✅ Correct parent class
    Package,
    *AssetName,
    BPTYPE_Normal,
    ...
);
```

**Generated Blueprint Structure:**
```
BP_StationDesign_MyStation (ASpaceStation)
├── Modules[] (auto-populated array)
├── ChildActorComponent_Module1 (BP_DockingBay)
├── ChildActorComponent_Module2 (BP_Reactor)
└── ChildActorComponent_Module3 (BP_Marketplace)
```

### Fallback (Standalone)

```cpp
// Creates AActor Blueprint
UBlueprint* NewBlueprint = FKismetEditorUtilities::CreateBlueprint(
    AActor::StaticClass(),  // ⚠️ Limited functionality
    ...
);
```

---

## Expected Adastrea Modules

The plugin expects to find these Blueprint types in the Adastrea project:

### Core Modules (MVP)
- `BP_SpaceStationModule_DockingBay` (Docking group, +50 power)
- `BP_SpaceStationModule_DockingPort` (Docking group, +10 power)
- `BP_SpaceStationModule_Marketplace` (Public group, +40 power)
- `BP_CargoBayModule` (Storage group, +20 power)
- `BP_SpaceStationModule_Habitation` (Habitation group, +30 power)
- `BP_SpaceStationModule_Corridor` (Connection group, +5 power)

### Power Modules (Post-MVP)
- `BP_ReactorModule` (Power group, -500 power generation)
- `BP_SolarArrayModule` (Power group, -200 power generation)

### Other Modules
- Any Blueprint inheriting from `ASpaceStationModule`

---

## Validation Rules

The plugin validates stations using Adastrea's requirements:

### Required (MVP)
- ✅ At least one Docking module (enables ship interaction)
- ✅ All modules connected (no isolated modules)

### Recommended
- ⚠️ Marketplace module (enables trading)
- ⚠️ Power balance (generation ≥ consumption)
- ⚠️ Storage modules (cargo management)

---

## Integration Checklist

For developers integrating the plugin with Adastrea:

- [x] Add `Adastrea` module dependency to Build.cs files
- [x] Update module discovery to check `ASpaceStationModule` inheritance
- [x] Read module properties from Blueprint CDOs
- [x] Export to `ASpaceStation` Blueprints
- [x] Use Adastrea's `EStationModuleGroup` enum
- [x] Test module discovery with Adastrea Blueprints
- [x] Test Blueprint export and loading in Adastrea
- [x] Verify station validation rules

---

## Testing

### Unit Tests
1. **Module Discovery**: Verify all Adastrea modules are discovered
2. **Property Reading**: Check module metadata is correctly read
3. **Blueprint Export**: Ensure exported Blueprints are `ASpaceStation` type
4. **Validation**: Test station validation rules

### Integration Tests
1. Place plugin in `Adastrea/Plugins/` directory
2. Open Adastrea project in Unreal Engine
3. Enable "Modular Station Designer" plugin
4. Open Station Builder (Tools → Station Builder)
5. Verify module palette shows Adastrea modules
6. Create test station and export
7. Load exported Blueprint in Adastrea level
8. Verify station spawns correctly with all modules

---

## Troubleshooting

### Module Discovery Issues

**Problem**: No modules discovered or wrong modules discovered

**Solution**:
1. Check that Adastrea module dependency is in Build.cs
2. Verify `ADASTREA_INTEGRATION_ENABLED` is defined (check build output)
3. Ensure Adastrea Blueprints inherit from `ASpaceStationModule`
4. Check Asset Registry has finished scanning (may take time on first load)

### Export Issues

**Problem**: Exported Blueprints don't work in Adastrea

**Solution**:
1. Verify exported Blueprint parent class is `ASpaceStation`
2. Check Child Actor Components reference valid module Blueprints
3. Ensure module transforms are correct
4. Validate station has required modules (Docking)

### Build Errors

**Problem**: Compilation errors about missing Adastrea classes

**Solution**:
1. Ensure Adastrea source is available at build time
2. Check Build.cs has correct module dependencies
3. Verify include paths: `#include "Stations/SpaceStationModule.h"`
4. Regenerate project files if needed

---

## API Reference

### Key Classes

#### `ASpaceStationModule` (Adastrea)
Base class for all station modules
- `ModuleType` (FString): Module identifier
- `ModulePower` (float): Power consumption/generation
- `ModuleGroup` (EStationModuleGroup): Functional category

#### `ASpaceStation` (Adastrea)
Base class for assembled stations
- `Modules` (TArray): All attached modules
- `GetMarketplaceModule()`: Find trading module
- `GetDockingBayModule()`: Find docking module
- `GetPowerBalance()`: Check power status

#### `FModuleDiscovery` (Plugin)
Module scanning and discovery system
- `DiscoverModules()`: Find all station modules
- `DiscoverModulesFiltered()`: Filter by group
- `ExtractModuleInfo()`: Read module metadata

#### `FStationExporter` (Plugin)
Blueprint generation and file I/O
- `ExportToBlueprintAsset()`: Create Adastrea Blueprint
- `SaveDesignToFile()`: JSON serialization
- `LoadDesignFromFile()`: JSON deserialization

---

## Future Enhancements

### Planned Features
- [ ] Connection point validation (size/type matching)
- [ ] Advanced module queries (by type, capabilities)
- [ ] Multi-faction station support
- [ ] Dynamic module construction at runtime
- [ ] Station modification gameplay

### Compatibility Roadmap
- [ ] Support for custom module types
- [ ] Plugin API for third-party modules
- [ ] Hot-reload of module definitions
- [ ] Blueprint interface for custom validation rules

---

## References

- [Adastrea Repository](https://github.com/Mittenzx/Adastrea)
- [Station Builder Specification](../STATION_BUILDER_PLUGIN_SPECIFICATION.md)
- [Architecture Documentation](ARCHITECTURE.md)
- [Contributing Guide](../CONTRIBUTING.md)

---

**Document Version**: 1.0  
**Compatibility**: Adastrea UE 5.6+  
**Plugin Version**: 1.0.0
