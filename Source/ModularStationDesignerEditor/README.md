# Editor Module - ModularStationDesignerEditor

This directory contains the editor module for the Modular Station Designer plugin.

## Structure

- `Public/` - Public header files for editor functionality
- `Private/` - Private implementation files for editor tools

## Key Components

### Main Window (StationDesignerWindow.h/cpp)
`SStationDesignerWindow` - Main plugin window (Slate widget):
- Toolbar with commands (New, Load, Save, Export, Validate)
- Module palette panel
- 3D viewport
- Properties panel
- Status bar

### Module Palette (ModulePalette.h/cpp)
`SModulePalette` - Module library and selection (Slate widget):
- Displays available modules
- Filters by group/search
- Drag-and-drop initiation
- Module metadata display

### Station Viewport (StationViewport.h/cpp)
`SStationViewport` - 3D visualization workspace (Slate widget):
- Renders station preview
- Camera controls
- Module manipulation
- Connection point visualization
- Snapping feedback

### Validation (StationValidator.h/cpp)
`FStationValidator` - Design validation logic:
- Checks required modules
- Verifies connectivity
- Calculates power balance
- Generates validation messages

### Export System (StationExporter.h/cpp)
`FStationExporter` - Blueprint generation:
- Creates Blueprint assets
- Adds Child Actor Components
- Sets transforms
- Saves to disk

### Module Discovery (ModuleDiscovery.h/cpp)
`FModuleDiscovery` - Scans Adastrea modules:
- Queries Asset Registry
- Filters module Blueprints
- Extracts metadata
- Caches module information

## Development Status

📝 This module is currently in the planning phase. Implementation will begin in Phase 1, Week 2 of the roadmap.

See [ROADMAP.md](../../ROADMAP.md) for the complete development timeline.
