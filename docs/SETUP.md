# Development Setup Guide

Complete guide for setting up your development environment for the Modular Station Designer plugin.

**Last Updated**: 2026-01-19  
**Plugin Version**: 0.1.0-alpha (In Development)

---

## Overview

This guide covers setting up the plugin for development. The plugin is currently in active development (Phase 1), so this guide focuses on getting the development environment ready for implementation work.

---

## Prerequisites

### Required Software

1. **Unreal Engine 5.4+** (5.6 recommended)
   - Download from [Epic Games Launcher](https://www.unrealengine.com/)
   - Install C++ development components

2. **Visual Studio 2022** (Windows) or **Xcode 14+** (Mac)
   - Windows: Install "Game Development with C++" workload
   - Mac: Install command line tools

3. **Git**
   - Download from [git-scm.com](https://git-scm.com/)

4. **Adastrea Project** (optional, for integration testing)
   - Clone from [Adastrea repository](https://github.com/Mittenzx/Adastrea)
   - Not strictly required for plugin development, but recommended for testing with actual Adastrea modules

### Optional Tools

- **Visual Studio Code** - Lightweight code editor
- **ReSharper C++** - Advanced C++ tools for Visual Studio
- **Unreal Insights** - Performance profiling

---

## Setup Instructions

### Option 1: Standalone Plugin Development

Best for developing the plugin independently.

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Mittenzx/AdastreaStationEditor.git
   cd AdastreaStationEditor
   ```

2. **Create test project** (if not using Adastrea):
   ```bash
   # Create new blank Unreal project
   # In Epic Games Launcher:
   # - New Project → Games → Blank
   # - Blueprint or C++: C++
   # - Target Platform: Desktop
   # - Name: StationBuilderTest
   ```

3. **Link plugin to project**:
   ```bash
   # Copy or symlink plugin to project's Plugins directory
   cd /path/to/StationBuilderTest
   mkdir Plugins
   
   # Windows (as administrator):
   mklink /D Plugins\ModularStationDesigner C:\path\to\AdastreaStationEditor
   
   # Mac/Linux:
   ln -s /path/to/AdastreaStationEditor Plugins/ModularStationDesigner
   ```

4. **Generate project files**:
   ```bash
   # Windows: Right-click .uproject → "Generate Visual Studio project files"
   # Mac: Right-click .uproject → "Generate Xcode project"
   ```

5. **Build and run**:
   - Open `.sln` (Windows) or `.xcodeproj` (Mac)
   - Set build configuration to "Development Editor"
   - Build solution (Ctrl+Shift+B or Cmd+B)
   - Run (F5)

### Option 2: Development within Adastrea Project

Best for testing integration with actual Adastrea modules.

1. **Clone Adastrea project**:
   ```bash
   git clone https://github.com/Mittenzx/Adastrea.git
   cd Adastrea
   ```

2. **Clone plugin into Plugins directory**:
   ```bash
   cd Plugins
   git clone https://github.com/Mittenzx/AdastreaStationEditor.git ModularStationDesigner
   ```

3. **Generate project files**:
   ```bash
   # Right-click Adastrea.uproject → "Generate project files"
   ```

4. **Build and run**:
   - Open Adastrea solution
   - Build "Development Editor" configuration
   - Launch Adastrea editor

5. **Enable plugin**:
   - Edit → Plugins
   - Search "Modular Station Designer"
   - Enable checkbox
   - Restart editor

---

## Verifying Setup

### 1. Check Plugin Structure

Verify the repository was cloned correctly:
```bash
cd /path/to/ModularStationDesigner
ls -la
# Should see: Source/, Content/, ModularStationDesigner.uplugin, README.md, etc.
```

### 2. Check Plugin Descriptor

Verify the `.uplugin` file is valid:
```bash
cat ModularStationDesigner.uplugin
# Should be valid JSON with "Modules" array
```

### 3. Check Build Files

Ensure build configuration files exist:
```bash
ls Source/ModularStationDesigner/ModularStationDesigner.Build.cs
ls Source/ModularStationDesignerEditor/ModularStationDesignerEditor.Build.cs
```

### 4. Check Plugin Load (After Building)

Once the plugin is built, check the Output Log in UE Editor (Window → Developer Tools → Output Log):
```
LogPluginManager: Mounting plugin ModularStationDesigner
```

No errors should appear. If you see errors, check the error message for details.

### 5. Verify Plugin in Editor

Once built and loaded:
- Edit → Plugins
- Search "Modular Station Designer"
- Plugin should appear in the list
- Enable checkbox should be available

---

## Development Workflow

### Daily Workflow

1. **Update repository**:
   ```bash
   git pull origin main
   ```

2. **Create feature branch**:
   ```bash
   git checkout -b feature/your-feature-name
   ```

3. **Make changes**:
   - Edit code in IDE
   - Save files

4. **Hot reload** (if engine is running):
   - Unreal Editor: Ctrl+Alt+F11 (compile)
   - Or close editor and rebuild solution

5. **Test changes**:
   - Launch editor
   - Test functionality
   - Check Output Log for errors

6. **Commit changes**:
   ```bash
   git add .
   git commit -m "feat: add your feature"
   git push origin feature/your-feature-name
   ```

7. **Open Pull Request** on GitHub

### Build Configurations

- **DebugGame Editor**: Full debugging, slower
- **Development Editor**: Balanced (recommended)
- **Shipping**: Optimized, no debug symbols

### Common Commands

```bash
# Clean build (Windows)
# Delete: Binaries/, Intermediate/, Saved/
# Then regenerate project files

# Build from command line (Windows)
"C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" ^
    ModularStationDesignerEditor Win64 Development ^
    "C:\path\to\Project.uproject" -WaitMutex

# Run tests from command line
"C:\path\to\Project\Binaries\Win64\ProjectEditor.exe" ^
    -ExecCmds="Automation RunTests StationBuilder" -unattended
```

---

## Troubleshooting

### Plugin doesn't appear in list

**Solution**:
1. Check `.uplugin` file is valid JSON
2. Verify module names match in `.uplugin` and `.Build.cs` files
3. Try deleting `Binaries/` and `Intermediate/` folders
4. Regenerate project files

### Build errors

**Solution**:
1. Check Unreal Engine version matches (5.4+)
2. Verify all dependencies listed in `.Build.cs`
3. Clean build directories
4. Update Unreal Engine if needed

### Editor crashes on startup

**Solution**:
1. Check Output Log before crash
2. Disable plugin, restart, re-enable
3. Verify no code errors in plugin module initialization
4. Check for null pointer dereferences

### Hot reload fails

**Solution**:
1. Close editor completely
2. Clean build
3. Rebuild solution
4. Restart editor
5. Hot reload has limitations - full restart often needed

---

## IDE Configuration

### Visual Studio

**Recommended extensions**:
- Visual Assist
- ReSharper C++
- UnrealVS Extension (included with UE)

**Useful settings**:
- Tools → Options → Text Editor → C++ → Advanced → IntelliSense → Disable Squiggles: True
- (Unreal's code can confuse IntelliSense)

### Visual Studio Code

**Recommended extensions**:
- C/C++ (Microsoft)
- Unreal Engine 4 Snippets
- Unreal Engine Intellisense

**Setup**:
1. Generate VSCode project files:
   ```bash
   # Add to .uproject:
   "GenerateProjectFiles": {
       "Format": "VisualStudioCode"
   }
   ```

### Rider

**Configuration**:
- File → Settings → Build, Execution, Deployment → Toolchain → Unreal Engine
- Set UE installation path
- Enable Unreal Engine support

---

## Performance Profiling

### Unreal Insights

1. Launch with profiling:
   ```bash
   ProjectEditor.exe -trace=cpu,gpu,frame
   ```

2. Open Unreal Insights:
   ```bash
   Engine/Binaries/Win64/UnrealInsights.exe
   ```

3. Analyze performance:
   - CPU usage
   - Memory allocation
   - Frame timing

### Memory Leaks

Use Unreal's memory profiler:
- Window → Developer Tools → Memory Profiler
- Snapshot before/after operations
- Look for growing allocations

---

## Additional Resources

### Documentation

- [Unreal Engine C++ Programming](https://docs.unrealengine.com/5.0/en-US/programming-with-cplusplus-in-unreal-engine/)
- [Plugin Development](https://docs.unrealengine.com/5.0/en-US/plugins-in-unreal-engine/)
- [Slate UI Framework](https://docs.unrealengine.com/5.0/en-US/slate-ui-framework-in-unreal-engine/)

### Community

- [Unreal Engine Forums](https://forums.unrealengine.com/)
- [Unreal Slackers Discord](https://unrealslackers.org/)
- [Stack Overflow - Unreal Engine](https://stackoverflow.com/questions/tagged/unreal-engine)

---

## Next Steps

Once your environment is set up:

1. ✅ Review [ARCHITECTURE.md](ARCHITECTURE.md)
2. ✅ Check [ROADMAP.md](../ROADMAP.md) for current phase
3. ✅ Look for "good first issue" labels on GitHub
4. ✅ Join discussions in Issues/PRs

Happy coding!

---

**Document Version**: 1.1  
**Last Updated**: 2026-01-19  
**Status**: Active - Updated for development phase
