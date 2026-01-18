# Quick Reference Guide

Quick reference for developers working on the Modular Station Designer plugin.

## Project Overview

- **Purpose**: Visual station builder for Adastrea space station game
- **Target**: Unreal Engine 5.6 (minimum 5.4)
- **Language**: C++ with Slate UI framework
- **Architecture**: Two-module plugin (Runtime + Editor)

---

## Repository Structure

```
AdastreaStationEditor/
├── .github/
│   └── ISSUE_TEMPLATE/          # GitHub issue templates
├── Source/
│   ├── ModularStationDesigner/  # Runtime module
│   └── ModularStationDesignerEditor/  # Editor module
├── Content/                      # Assets (icons, thumbnails)
├── Config/                       # Configuration files
├── docs/                         # Documentation
│   ├── ARCHITECTURE.md          # Technical architecture
│   ├── SETUP.md                 # Development setup
│   └── QUICK_REFERENCE.md       # This file
├── ModularStationDesigner.uplugin  # Plugin descriptor
├── README.md                    # Project overview
├── ROADMAP.md                   # Development timeline
├── CONTRIBUTING.md              # Contribution guidelines
├── .gitignore                   # Git ignore rules
└── STATION_BUILDER_PLUGIN_SPECIFICATION.md  # Full specification
```

---

## Key Files

### Plugin Configuration
- **ModularStationDesigner.uplugin** - Plugin metadata and module definitions
- **Source/*/[Module].Build.cs** - Build configuration for each module

### Core Documentation
- **README.md** - Start here for project overview
- **ROADMAP.md** - Development phases and timeline
- **ARCHITECTURE.md** - Technical design and structure
- **SETUP.md** - Development environment setup

### Specification
- **STATION_BUILDER_PLUGIN_SPECIFICATION.md** - Complete feature spec (46KB, very detailed)

---

## Key Concepts

### Modules
- **Runtime Module** (`ModularStationDesigner`): Core data structures, connection logic
- **Editor Module** (`ModularStationDesignerEditor`): UI, validation, export system

### Core Classes (To Be Implemented)

**Runtime**:
- `UConnectionPointComponent` - Attachment points on modules
- `FModulePlacement` - Module position/metadata
- `FStationDesign` - Complete station layout

**Editor**:
- `SStationDesignerWindow` - Main UI window (Slate)
- `SModulePalette` - Module library panel
- `FStationValidator` - Design validation
- `FStationExporter` - Blueprint generation
- `FModuleDiscovery` - Adastrea module scanning

### Integration with Adastrea
- Discovers modules from `ASpaceStationModule` descendants
- Exports as Blueprint with `ASpaceStation` parent class
- Uses Child Actor Components for module placement

---

## Development Phases

### Phase 0: Project Setup (Current)
- Repository structure ✅
- Documentation ✅
- Planning complete ✅

### Phase 1: Core Functionality (Weeks 1-4)
- Week 1: Plugin structure, build system
- Week 2: Module discovery
- Week 3: Basic UI
- Week 4: Connection system

### Phase 2: Export & Polish (Weeks 5-8)
- Week 5: Export to Blueprint
- Week 6: Validation system
- Week 7: Save/load
- Week 8: Polish and testing

### Phase 3: Advanced Features (Weeks 9-12)
- Week 9: Templates
- Week 10: Advanced tools
- Week 11: Visualization
- Week 12: Release preparation

---

## Common Commands

### Git Workflow
```bash
# Update main branch
git checkout main
git pull origin main

# Create feature branch
git checkout -b feature/your-feature-name

# Commit changes
git add .
git commit -m "feat: description of change"

# Push branch
git push origin feature/your-feature-name
```

### Build Commands (Windows)
```bash
# Clean build
# Delete: Binaries/, Intermediate/, Saved/
# Right-click .uproject → Generate Visual Studio project files

# Build in Visual Studio
# Configuration: Development Editor
# Ctrl+Shift+B to build

# Run from Visual Studio
# F5 or Ctrl+F5
```

### Build Commands (Command Line)
```bash
# Build plugin
UnrealBuildTool.exe ModularStationDesignerEditor Win64 Development ^
    "C:\path\to\Project.uproject"

# Run tests
ProjectEditor.exe -ExecCmds="Automation RunTests StationBuilder" -unattended
```

---

## Coding Standards

### Naming Conventions
```cpp
// Classes
class UConnectionPointComponent;  // U prefix for UObject
class FStationValidator;           // F prefix for non-UObject
class SModulePalette;              // S prefix for Slate widgets

// Functions
void DiscoverModules();            // PascalCase
bool CanConnectTo(...);

// Variables
UPROPERTY()
FString ModuleName;                // PascalCase for properties
float localVariable;               // camelCase for locals
bool bIsConnected;                 // 'b' prefix for booleans

// Constants
const int32 kMaxModules = 100;     // 'k' prefix or ALL_CAPS
```

### File Organization
- One class per file (generally)
- Header files: `.h` extension
- Implementation files: `.cpp` extension
- Use `#pragma once` for include guards

---

## Testing

### Manual Testing Checklist
- [ ] Plugin loads without errors
- [ ] Main window opens correctly
- [ ] Module palette displays modules
- [ ] Can drag and place modules
- [ ] Connection snapping works
- [ ] Validation provides feedback
- [ ] Export creates valid Blueprint
- [ ] Exported station works in Adastrea

### Unit Test Template
```cpp
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FYourTest,
    "StationBuilder.Category.TestName",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FYourTest::RunTest(const FString& Parameters)
{
    // Arrange
    // ...
    
    // Act
    // ...
    
    // Assert
    TestTrue("Description", bCondition);
    
    return true;
}
```

---

## Useful Links

### Unreal Engine Documentation
- [C++ Programming Guide](https://docs.unrealengine.com/5.0/en-US/programming-with-cplusplus-in-unreal-engine/)
- [Plugin Development](https://docs.unrealengine.com/5.0/en-US/plugins-in-unreal-engine/)
- [Slate UI Framework](https://docs.unrealengine.com/5.0/en-US/slate-ui-framework-in-unreal-engine/)
- [Blueprint API](https://docs.unrealengine.com/5.0/en-US/BlueprintAPI/)

### Project Links
- [Repository](https://github.com/Mittenzx/AdastreaStationEditor)
- [Issues](https://github.com/Mittenzx/AdastreaStationEditor/issues)
- [Adastrea Project](https://github.com/Mittenzx/Adastrea)

---

## Troubleshooting Quick Fixes

### Plugin not showing up
1. Check `.uplugin` file is valid JSON
2. Verify module names match
3. Delete `Binaries/` and `Intermediate/`
4. Regenerate project files

### Build fails
1. Check UE version (5.4+)
2. Verify dependencies in `.Build.cs`
3. Clean and rebuild
4. Update Unreal Engine

### Editor crashes
1. Check Output Log
2. Disable plugin and re-enable
3. Check for null pointers
4. Rebuild in Debug configuration

---

## Performance Targets

- **Load Time**: < 2 seconds
- **Viewport FPS**: 60 FPS with 50+ modules
- **Export Time**: < 5 seconds
- **Memory**: < 500 MB overhead

---

## Next Steps for New Contributors

1. ✅ Read this guide
2. ✅ Review [SETUP.md](SETUP.md) and set up environment
3. ✅ Read [ARCHITECTURE.md](ARCHITECTURE.md)
4. ✅ Check [ROADMAP.md](../ROADMAP.md) for current phase
5. ✅ Look for "good first issue" labels
6. ✅ Join discussions on GitHub

---

**Version**: 1.0  
**Last Updated**: 2026-01-18  
**Quick Tip**: Keep this file open while developing!
