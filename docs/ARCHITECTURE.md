# Architecture Overview

## Modular Station Designer - Technical Architecture

**Version**: 1.0  
**Last Updated**: 2026-01-18

---

## System Architecture

### High-Level Overview

The Modular Station Designer is built as an Unreal Engine 5 plugin with two main modules:

```
┌─────────────────────────────────────────────────────────────┐
│                  Unreal Engine 5.6                          │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌───────────────────────────────────────────────────┐    │
│  │     ModularStationDesignerEditor (Editor Module)   │    │
│  │  ┌─────────────────────────────────────────────┐  │    │
│  │  │  Station Designer Window (Slate UI)         │  │    │
│  │  │  - Module Palette                          │  │    │
│  │  │  - 3D Viewport                             │  │    │
│  │  │  - Properties Panel                        │  │    │
│  │  └─────────────────────────────────────────────┘  │    │
│  │  ┌─────────────────────────────────────────────┐  │    │
│  │  │  Core Systems                              │  │    │
│  │  │  - Module Discovery                        │  │    │
│  │  │  - Validation Engine                       │  │    │
│  │  │  - Export System                           │  │    │
│  │  └─────────────────────────────────────────────┘  │    │
│  └───────────────────────────────────────────────────┘    │
│                         ↕                                   │
│  ┌───────────────────────────────────────────────────┐    │
│  │   ModularStationDesigner (Runtime Module)         │    │
│  │  - Connection Point Components                    │    │
│  │  - Module Attachment Logic                        │    │
│  │  - Station Data Types                             │    │
│  └───────────────────────────────────────────────────┘    │
│                         ↕                                   │
│  ┌───────────────────────────────────────────────────┐    │
│  │        Adastrea Project                           │    │
│  │  - ASpaceStationModule (Base Class)               │    │
│  │  - Module Blueprints                              │    │
│  │  - ASpaceStation (Parent Class)                   │    │
│  └───────────────────────────────────────────────────┘    │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## Module Architecture

### Runtime Module: ModularStationDesigner

**Purpose**: Core data structures and runtime components

**Key Classes**:

1. **UConnectionPointComponent**
   - Purpose: Represents attachment points on modules
   - Type: `USceneComponent`
   - Responsibilities:
     - Store connection type and size
     - Validate connection compatibility
     - Manage occupied/unoccupied state
     - Visual gizmo display

2. **FStationDesignerTypes**
   - Purpose: Data structures for station designs
   - Key Structs:
     - `FModulePlacement`: Module position and metadata
     - `FStationDesign`: Complete station layout
     - `EConnectionType`: Connection type enumeration
     - `EConnectionSize`: Connection size enumeration

3. **FModuleAttachment**
   - Purpose: Manage module-to-module connections
   - Responsibilities:
     - Track connection relationships
     - Enforce connection rules
     - Provide connection metadata

**Dependencies**:
- `Core`
- `CoreUObject`
- `Engine`

---

### Editor Module: ModularStationDesignerEditor

**Purpose**: Editor tools and UI

**Key Classes**:

1. **SStationDesignerWindow** (Slate Widget)
   - Purpose: Main plugin window
   - Components:
     - Toolbar (New, Load, Save, Export, Validate)
     - Module Palette panel
     - 3D Viewport
     - Properties panel
     - Status bar
   - Responsibilities:
     - Window layout management
     - Command handling
     - Design state management

2. **SModulePalette** (Slate Widget)
   - Purpose: Module library and selection
   - Responsibilities:
     - Display available modules
     - Filter by group/search
     - Drag-and-drop initiation
     - Module metadata display

3. **SStationViewport** (Slate Widget)
   - Purpose: 3D visualization workspace
   - Responsibilities:
     - Render station preview
     - Camera controls
     - Module manipulation
     - Connection point visualization
     - Snapping feedback

4. **FStationValidator**
   - Purpose: Design validation logic
   - Responsibilities:
     - Check required modules
     - Verify connectivity
     - Calculate power balance
     - Generate validation messages

5. **FStationExporter**
   - Purpose: Blueprint generation
   - Responsibilities:
     - Create Blueprint assets
     - Add Child Actor Components
     - Set transforms
     - Save to disk

6. **FModuleDiscovery**
   - Purpose: Scan Adastrea modules
   - Responsibilities:
     - Query Asset Registry
     - Filter module Blueprints
     - Extract metadata
     - Cache module information

**Dependencies**:
- Runtime module: `ModularStationDesigner`
- UI: `Slate`, `SlateCore`, `EditorStyle`
- Editor: `UnrealEd`, `LevelEditor`, `PropertyEditor`
- Assets: `AssetTools`

---

## Data Flow

### Module Discovery Flow

```
1. Plugin Startup
   ↓
2. FModuleDiscovery::DiscoverModules()
   ↓
3. Query Asset Registry for Blueprints
   ↓
4. Filter: IsChildOf(ASpaceStationModule)
   ↓
5. Extract Metadata (Name, Type, Group, Power)
   ↓
6. Load Thumbnails
   ↓
7. Populate Module Palette
```

### Design Creation Flow

```
1. User Opens Station Builder Window
   ↓
2. Module Palette Displays Available Modules
   ↓
3. User Drags Module to Viewport
   ↓
4. Module Placement Created (FModulePlacement)
   ↓
5. Module Added to Current Design (FStationDesign)
   ↓
6. Connection Points Visualized
   ↓
7. User Positions and Connects Modules
   ↓
8. Snapping System Adjusts Positions
   ↓
9. Connection Validation Provides Feedback
```

### Export Flow

```
1. User Clicks Export Button
   ↓
2. FStationValidator::ValidateStation()
   ├─ Check Required Modules
   ├─ Check Connectivity
   └─ Check Power Balance
   ↓
3. If Valid: Proceed
   ↓
4. FStationExporter::ExportToBlueprintAsset()
   ↓
5. Create Blueprint Asset
   ├─ Parent Class: ASpaceStation
   └─ Package Path: /Game/Blueprints/Stations/Generated/
   ↓
6. For Each Module in Design:
   ├─ Create Child Actor Component
   ├─ Set Child Actor Class
   └─ Set Transform
   ↓
7. Save Blueprint Asset
   ↓
8. Notify User of Success
```

---

## Key Design Patterns

### 1. Model-View-Controller (MVC)

- **Model**: `FStationDesign`, `FModulePlacement`
- **View**: Slate widgets (`SStationDesignerWindow`, `SModulePalette`, `SStationViewport`)
- **Controller**: Command handlers, user input processing

### 2. Component-Based Architecture

- Connection points as components (`UConnectionPointComponent`)
- Modular, reusable design
- Easy to extend with new component types

### 3. Observer Pattern

- Station design changes trigger UI updates
- Module selection updates properties panel
- Validation results update status bar

### 4. Factory Pattern

- Blueprint creation via `FStationExporter`
- Module info construction via `FModuleDiscovery`

---

## File Structure

```
ModularStationDesigner/
├── Source/
│   ├── ModularStationDesigner/              # Runtime module
│   │   ├── Public/
│   │   │   ├── StationDesignerTypes.h       # Data structures
│   │   │   ├── ConnectionPoint.h            # Connection component
│   │   │   └── ModuleAttachment.h           # Attachment logic
│   │   ├── Private/
│   │   │   ├── ConnectionPoint.cpp
│   │   │   ├── ModuleAttachment.cpp
│   │   │   └── ModularStationDesigner.cpp   # Module impl
│   │   └── ModularStationDesigner.Build.cs
│   │
│   └── ModularStationDesignerEditor/        # Editor module
│       ├── Public/
│       │   ├── StationDesignerWindow.h      # Main window
│       │   ├── ModulePalette.h              # Module library
│       │   ├── StationValidator.h           # Validation
│       │   ├── StationExporter.h            # Export system
│       │   └── ModuleDiscovery.h            # Module scanning
│       ├── Private/
│       │   ├── StationDesignerWindow.cpp
│       │   ├── ModulePalette.cpp
│       │   ├── StationViewport.cpp          # 3D viewport
│       │   ├── StationValidator.cpp
│       │   ├── StationExporter.cpp
│       │   ├── ModuleDiscovery.cpp
│       │   └── ModularStationDesignerEditor.cpp
│       └── ModularStationDesignerEditor.Build.cs
│
├── Content/
│   ├── Icons/                               # UI icons
│   ├── ModuleThumbnails/                    # Preview images
│   └── EditorAssets/                        # Editor-only assets
│
├── Config/
│   └── FilterPlugin.ini                     # Plugin configuration
│
├── docs/
│   ├── ARCHITECTURE.md                      # This file
│   ├── API_REFERENCE.md                     # API documentation
│   └── USER_GUIDE.md                        # User documentation
│
└── ModularStationDesigner.uplugin           # Plugin descriptor
```

---

## Integration with Adastrea

### Module Discovery

The plugin uses Unreal's Asset Registry to discover all Blueprint classes that inherit from `ASpaceStationModule`:

```cpp
void FModuleDiscovery::DiscoverModules()
{
    FAssetRegistryModule& AssetRegistryModule = 
        FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
    
    TArray<FAssetData> BlueprintAssets;
    AssetRegistry.GetAssetsByClass(UBlueprint::StaticClass()->GetFName(), BlueprintAssets);
    
    for (const FAssetData& AssetData : BlueprintAssets)
    {
        UBlueprint* Blueprint = Cast<UBlueprint>(AssetData.GetAsset());
        if (Blueprint && Blueprint->GeneratedClass &&
            Blueprint->GeneratedClass->IsChildOf(ASpaceStationModule::StaticClass()))
        {
            AddModuleToPalette(Blueprint);
        }
    }
}
```

### Blueprint Export

Generated Blueprints use Child Actor Components to reference module Blueprints:

```cpp
bool FStationExporter::ExportToBlueprintAsset(const FStationDesign& Design)
{
    // Create Blueprint asset
    UBlueprint* NewBlueprint = CreateBlueprintAsset(PackagePath, AssetName);
    NewBlueprint->ParentClass = ASpaceStation::StaticClass();
    
    // Add Child Actor Components for each module
    for (const FModulePlacement& Module : Design.Modules)
    {
        UChildActorComponent* Component = NewObject<UChildActorComponent>(
            NewBlueprint->SimpleConstructionScript->GetDefaultSceneRootNode());
        Component->SetChildActorClass(Module.ModuleBlueprintPath.TryLoadClass<AActor>());
        Component->SetRelativeTransform(Module.Transform);
    }
    
    return true;
}
```

---

## Performance Considerations

### Rendering Optimization
- **LOD System**: Distant modules use simplified visuals
- **Culling**: Off-screen connection points not rendered
- **Batching**: Module thumbnails loaded asynchronously

### Memory Management
- **Module Cache**: Metadata cached to avoid repeated Asset Registry queries
- **Lazy Loading**: Module Blueprints loaded on-demand
- **Weak References**: Use soft object references for Blueprint classes

### UI Responsiveness
- **Async Operations**: Module discovery runs on background thread
- **Update Throttling**: Viewport updates at maximum 60 FPS
- **Event Batching**: Multiple changes trigger single UI update

---

## Extension Points

### Adding New Module Types

1. Create new Blueprint inheriting from `ASpaceStationModule`
2. Add `UConnectionPointComponent` instances to Blueprint
3. Set module properties (Type, Group, Power)
4. Plugin automatically discovers and displays new module

### Custom Connection Types

1. Add new value to `EConnectionType` enum
2. Update `UConnectionPointComponent::CanConnectTo()` with compatibility rules
3. Add visual indicator for new type

### Custom Validation Rules

1. Create new validation function in `FStationValidator`
2. Add to `ValidateStation()` call chain
3. Return `FValidationMessage` with appropriate severity

---

## Security Considerations

### Asset Loading
- Validate Blueprint classes before loading
- Handle missing or corrupt assets gracefully
- Prevent loading of non-module classes

### Export Safety
- Validate export path is within project directory
- Check for file overwrites
- Sanitize user-provided names

### Connection Validation
- Enforce connection rules strictly
- Prevent invalid module configurations
- Validate all data before export

---

## Testing Strategy

### Unit Tests
- Connection compatibility rules
- Validation logic
- Data serialization/deserialization

### Integration Tests
- Module discovery with real Adastrea project
- Blueprint export and import
- End-to-end workflow testing

### Performance Tests
- Large station designs (100+ modules)
- Rapid module placement/deletion
- Export time benchmarks

---

## Future Architecture Considerations

### Scalability
- Support for very large stations (500+ modules)
- Distributed computation for validation
- Cloud-based template storage

### Extensibility
- Plugin system for custom tools
- Scripting API for automation
- Third-party integration hooks

### Collaboration
- Multi-user editing support
- Version control integration
- Change tracking and history

---

**Document Status**: Active  
**Last Review**: 2026-01-18  
**Next Review**: End of Phase 1
