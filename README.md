# Modular Station Designer for Adastrea

A visual station builder plugin for Unreal Engine that enables drag-and-drop modular station construction with connection points, snapping, and direct export to Adastrea's editor.

## Overview

This plugin provides a streamlined workflow for designing space stations in the Adastrea project. The goal is to enable designers to visually construct modular space stations instead of manually placing and positioning modules in the Unreal Editor.

### Planned Features

- 🎯 **Intuitive Interface**: Visual 3D workspace with drag-and-drop module placement (Planned)
- 🔌 **Smart Connections**: Automatic snapping and validation of module connections (In Development)
- ✅ **Built-in Validation**: Check for required modules, power balance, and connectivity (Planned)
- 📦 **Direct Export**: Generate Blueprint actors ready for use in Adastrea (Planned)
- 💾 **Save/Load**: Preserve designs for later editing (Planned)
- 🚀 **Fast Workflow**: Target 5-10x faster than manual placement (Goal)

## Target Users

- **Level Designers** building space stations for Adastrea
- **Content Creators** designing custom station layouts
- **Game Developers** prototyping station configurations

## System Requirements

- **Unreal Engine**: 5.4+ (5.6 recommended)
- **Platform**: Windows, Mac, or Linux
- **Dependencies**: Adastrea project modules
- **Build Tools**: Visual Studio 2022 (Windows) or Xcode 14+ (Mac)

## Installation

### For Unreal Engine (UE)

#### Option 1: Plugin Directory Installation (Recommended for Development)

1. **Clone the repository:**
   ```bash
   cd /path/to/your/UnrealProject/Plugins
   git clone https://github.com/Mittenzx/AdastreaStationEditor.git ModularStationDesigner
   ```

2. **Generate project files:**
   - Right-click on your `.uproject` file
   - Select "Generate Visual Studio project files" (Windows) or "Generate Xcode project" (Mac)

3. **Build the plugin:**
   - Open the generated solution file (`.sln` or `.xcodeproj`)
   - Set build configuration to "Development Editor"
   - Build the solution (Ctrl+Shift+B on Windows, Cmd+B on Mac)

4. **Enable the plugin:**
   - Launch Unreal Engine Editor
   - Go to Edit → Plugins
   - Search for "Modular Station Designer"
   - Check the "Enabled" checkbox
   - Restart the editor when prompted

#### Option 2: Engine Plugins Directory (System-Wide)

1. **Clone to engine plugins:**
   ```bash
   cd /path/to/UnrealEngine/Engine/Plugins
   git clone https://github.com/Mittenzx/AdastreaStationEditor.git ModularStationDesigner
   ```

2. **Rebuild the engine** (if using source version) or regenerate project files for your project

3. **Enable the plugin** in your project as described in Option 1, step 4

### For CCO (Unreal Editor)
- **Unreal Engine**: 5.6 (primary), 5.4+ (minimum)
- **Platform**: Windows (primary), Mac/Linux (optional)
- **Dependencies**: Adastrea project modules (optional - enables full integration)

## Adastrea Integration

This plugin supports **optional integration** with the Adastrea game project:

**With Adastrea (Full Integration Mode):**
- ✅ Discovers modules by checking `ASpaceStationModule` inheritance
- ✅ Reads actual module properties (type, power, group) from Blueprints
- ✅ Exports to `ASpaceStation` Blueprints ready for Adastrea
- ✅ Uses Adastrea's module group system
- ✅ Validates stations against Adastrea requirements

**Standalone Mode (Fallback):**
- ⚠️ Name-based module discovery
- ⚠️ Inferred module properties
- ⚠️ Exports to generic `AActor` Blueprints

See [Adastrea Compatibility Guide](docs/ADASTREA_COMPATIBILITY.md) for detailed integration information.

CCO stands for "Content Creation Only" - using the Unreal Editor without building from source.

1. **Download or clone the plugin:**
   ```bash
   # Navigate to your project's Plugins directory
   cd /path/to/YourProject/Plugins
   git clone https://github.com/Mittenzx/AdastreaStationEditor.git ModularStationDesigner
   ```

2. **Open your project:**
   - Launch Unreal Engine from Epic Games Launcher
   - Open your Adastrea project
   - If prompted about missing modules, click "Yes" to rebuild

3. **Enable the plugin:**
   - Go to Edit → Plugins
   - Search for "Modular Station Designer"
   - Enable the plugin
   - Restart the editor

4. **Verify installation:**
   - Check the Output Log (Window → Developer Tools → Output Log)
   - Look for: `LogPluginManager: Mounting plugin ModularStationDesigner`
   - No errors should appear

### Verification

Once installed, you can verify the plugin is working:

1. Open the Output Log: Window → Developer Tools → Output Log
2. Search for "ModularStationDesigner" or "StationBuilder"
3. You should see messages indicating the plugin loaded successfully

**Note:** The plugin is currently in active development. The UI and features are being implemented according to the [roadmap](ROADMAP.md).

## Project Status

**Current Phase**: 🚧 Phase 1 - Foundation & Core Structure (In Progress)

**What's Complete:**
- ✅ Repository structure and comprehensive documentation
- ✅ Plugin architecture and module structure designed
- ✅ Core C++ classes created (connection points, validators, exporters)
- ✅ Build configuration (`.Build.cs` files)
- ✅ Plugin descriptor (`.uplugin`)

**In Progress:**
- 🚧 Core feature implementation
- 🚧 Module discovery system
- 🚧 Connection point behavior
- 🚧 User interface implementation

**Planned:**
- 📋 Phase 2: Export system, validation, save/load
- 📋 Phase 3: Templates, advanced tools, visualization

**Current Version:** 0.1.0-alpha (Development)

See [ROADMAP.md](ROADMAP.md) for detailed development timeline and progress.

## Documentation

- [Roadmap](ROADMAP.md) - Development phases, timeline, and current progress
- [Architecture](docs/ARCHITECTURE.md) - Technical design and structure
- [Setup Guide](docs/SETUP.md) - Development environment setup
- [User Guide](docs/USER_GUIDE.md) - Usage instructions (for future features)
- [Adastrea Compatibility](docs/ADASTREA_COMPATIBILITY.md) - Integration with Adastrea game modules
- [Contributing](CONTRIBUTING.md) - How to contribute to the project
- [Station Builder Specification](STATION_BUILDER_PLUGIN_SPECIFICATION.md) - Complete feature specification

## Development Timeline

- **Phase 0** (Complete): Repository setup and documentation
- **Phase 1** (Weeks 1-4, In Progress): Core functionality - Module discovery, basic UI, connection system
- **Phase 2** (Weeks 5-8, Planned): Export & polish - Blueprint generation, validation, save/load
- **Phase 3** (Weeks 9-12, Planned): Advanced features - Templates, advanced tools, visualization

## Target Success Metrics

- 5-10x faster than manual module placement
- 60 FPS performance with 50+ modules
- < 5 second export time for typical stations
- < 2 second plugin load time

## Troubleshooting

### Plugin doesn't appear in the plugins list
- Verify the `.uplugin` file exists and is valid JSON
- Check that the plugin is in the correct directory (`Plugins/ModularStationDesigner/`)
- Regenerate project files

### Build errors
- Ensure you have Unreal Engine 5.4 or higher
- Verify Visual Studio 2022 (Windows) or Xcode 14+ (Mac) is installed
- Check that all dependencies in `.Build.cs` files are available
- Try cleaning the build (delete `Binaries/` and `Intermediate/` directories)

### Plugin fails to load
- Check the Output Log for specific error messages
- Verify the Adastrea project is set up correctly
- Ensure all module dependencies are met

For more detailed troubleshooting, see [docs/SETUP.md](docs/SETUP.md).

## Related Projects

- [Adastrea](https://github.com/Mittenzx/Adastrea) - The main space station game project

## License

This project is part of the Adastrea development effort.

## Contact

For questions or feedback, please open an issue on GitHub.

---

**Version**: 0.1.0-alpha  
**Last Updated**: 2026-01-19  
**Status**: In Active Development - Phase 1 🚧
