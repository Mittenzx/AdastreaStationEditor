# Quick Start Guide - Modular Station Designer

**Version**: 0.1.0-alpha  
**Last Updated**: 2026-01-19

---

## ⚠️ Development Notice

This plugin is currently **in active development**. Most features are planned but not yet fully implemented. This guide will help you install the plugin for development or testing purposes.

---

## 5-Minute Installation

### Prerequisites
- Unreal Engine 5.4 or higher installed
- Visual Studio 2022 (Windows) or Xcode 14+ (Mac) for C++ compilation
- An Unreal Engine C++ project (or willingness to create one)

---

## Installation Methods

### Method 1: Quick Install for Content Creators (CCO)

**For users who want to use the plugin without building from source:**

1. **Download the plugin:**
   ```bash
   cd YourProject/Plugins
   git clone https://github.com/Mittenzx/AdastreaStationEditor.git ModularStationDesigner
   ```

2. **Open your project:**
   - Launch Unreal Engine from Epic Games Launcher
   - Open your Adastrea project
   - Click "Yes" when prompted to rebuild missing modules

3. **Enable the plugin:**
   - Go to Edit → Plugins
   - Search for "Modular Station Designer"
   - Check the "Enabled" box
   - Restart the editor

4. **Verify:**
   - Check Output Log (Window → Developer Tools → Output Log)
   - Look for: `LogPluginManager: Mounting plugin ModularStationDesigner`

---

### Method 2: Developer Install

**For developers who want to modify or extend the plugin:**

1. **Clone to your project:**
   ```bash
   cd /path/to/YourUnrealProject/Plugins
   git clone https://github.com/Mittenzx/AdastreaStationEditor.git ModularStationDesigner
   ```

2. **Generate project files:**
   - Right-click your `.uproject` file
   - Select "Generate Visual Studio project files" (Windows) or "Generate Xcode project" (Mac)

3. **Build the plugin:**
   - Open the solution/project file
   - Set configuration to "Development Editor"
   - Build (Ctrl+Shift+B / Cmd+B)

4. **Launch editor:**
   - Run the project (F5)
   - Enable plugin in Edit → Plugins
   - Restart editor

---

## What Works Now

✅ **Currently Available:**
- Plugin structure and build configuration
- Core C++ classes (connection points, validators, exporters)
- Plugin loads in Unreal Editor
- Basic architecture for future features

🚧 **In Development (Phase 1):**
- Module discovery system
- Connection point behavior
- User interface
- Drag-and-drop functionality

📋 **Planned (Phase 2 & 3):**
- Export to Blueprint
- Validation system
- Save/Load stations
- Templates
- Advanced tools

---

## Current Limitations

Since the plugin is in early development:
- **No UI yet**: The Station Builder window is not yet implemented
- **No functionality**: Core features are being developed
- **For developers**: This is primarily for contributors at this stage

---

## Next Steps

### For Contributors:
1. Read [CONTRIBUTING.md](CONTRIBUTING.md)
2. Check [ROADMAP.md](ROADMAP.md) for current tasks
3. Review [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)
4. Look for issues labeled "good first issue"

### For End Users:
- **Wait for v0.5-beta** for basic usable features
- **Wait for v1.0** for full feature set
- Star the repository to follow progress
- Check back for updates

---

## Troubleshooting

### "Plugin failed to load"
- Check Output Log for specific error
- Ensure UE version is 5.4 or higher
- Verify all source files are present

### "Missing modules" error
- This is normal - click "Yes" to rebuild
- Ensure Visual Studio/Xcode is installed
- Check that C++ compilation tools are available

### Plugin doesn't appear in list
- Verify `.uplugin` file exists
- Check plugin is in `Plugins/ModularStationDesigner/` directory
- Regenerate project files

### Build errors
- Update to UE 5.4 or higher
- Verify C++ development tools installed
- Try cleaning build (delete `Binaries/`, `Intermediate/`)
- Regenerate project files

---

## Getting Help

- **Documentation**: Check [docs/](docs/) folder
- **Issues**: Report bugs on [GitHub Issues](https://github.com/Mittenzx/AdastreaStationEditor/issues)
- **Setup Help**: See [docs/SETUP.md](docs/SETUP.md)
- **Architecture**: See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md)

---

## Stay Updated

- **GitHub**: Watch the repository for updates
- **Roadmap**: Check [ROADMAP.md](ROADMAP.md) for progress
- **Releases**: Follow [GitHub Releases](https://github.com/Mittenzx/AdastreaStationEditor/releases)

---

**Status**: Development - Phase 1 (Foundation) 🚧  
**Contributors Welcome**: Yes! See [CONTRIBUTING.md](CONTRIBUTING.md)  
**Expected Beta**: Phase 2 completion (Week 8)  
**Expected v1.0**: Phase 3 completion (Week 12)
