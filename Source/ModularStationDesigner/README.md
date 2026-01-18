# Runtime Module - ModularStationDesigner

This directory contains the runtime module for the Modular Station Designer plugin.

## Structure

- `Public/` - Public header files accessible to other modules
- `Private/` - Private implementation files and internal headers

## Key Components

### Data Types (StationDesignerTypes.h)
Core data structures for station designs:
- `FModulePlacement` - Module position and metadata
- `FStationDesign` - Complete station layout
- `EConnectionType` - Connection type enumeration
- `EConnectionSize` - Connection size enumeration

### Connection Points (ConnectionPoint.h/cpp)
`UConnectionPointComponent` - Scene component representing attachment points on modules:
- Validates connection compatibility
- Manages occupied/unoccupied state
- Provides visual gizmo display

### Module Attachment (ModuleAttachment.h/cpp)
`FModuleAttachment` - Manages module-to-module connections:
- Tracks connection relationships
- Enforces connection rules
- Provides connection metadata

## Development Status

📝 This module is currently in the planning phase. Implementation will begin in Phase 1, Week 1 of the roadmap.

See [ROADMAP.md](../../ROADMAP.md) for the complete development timeline.
