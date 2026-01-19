# Modular Station Designer - User Guide

**Version**: 1.0  
**Last Updated**: 2026-01-18  
**Plugin Version**: 1.0.0

---

## Table of Contents

1. [Getting Started](#getting-started)
2. [Basic Operations](#basic-operations)
3. [Advanced Features](#advanced-features)
4. [Templates](#templates)
5. [Validation](#validation)
6. [Export](#export)
7. [Troubleshooting](#troubleshooting)

---

## Getting Started

### Opening the Station Builder

1. Open your Unreal Engine project
2. Navigate to **Tools → Station Builder** in the main menu
3. The Station Builder window will open

### Interface Overview

The Station Builder interface consists of:

- **Toolbar**: Main actions (New, Load, Save, Export, Validate)
- **Module Palette**: Available station modules organized by group
- **3D Viewport**: Main design workspace
- **Properties Panel**: Selected module properties
- **Status Bar**: Current station statistics

---

## Basic Operations

### Creating a New Station

1. Click **New** in the toolbar
2. Enter a station name
3. Begin adding modules from the palette

### Adding Modules

1. Select a module from the palette
2. Drag and drop into the 3D viewport
3. Module will snap to nearby connection points

### Connecting Modules

Modules automatically connect when:
- Connection points are within snap distance (50cm default)
- Connection types are compatible
- Connection sizes match or are adjacent

**Connection Types:**
- **Standard**: General purpose connections
- **Docking**: Docking bay connections
- **Corridor**: Hallway connections
- **Power**: Power transmission
- **Universal**: Connects to everything

**Connection Sizes:**
- **Small**: 2m diameter
- **Medium**: 4m diameter
- **Large**: 6m diameter
- **Universal**: Any size

### Removing Modules

1. Select module in viewport
2. Press **Delete** key
3. Or right-click → Remove

---

## Advanced Features

### Copy and Paste

**Copy:**
1. Select one or more modules
2. Press **Ctrl+C** or Edit → Copy

**Paste:**
1. Press **Ctrl+V** or Edit → Paste
2. Modules appear with offset from originals
3. Connections are cleared and must be re-established

### Mirror

1. Select modules to mirror
2. Tools → Mirror → Choose Axis (X, Y, or Z)
3. Mirrored modules appear on opposite side

### Rotate

1. Select modules to rotate
2. Tools → Rotate
3. Enter rotation angle in degrees
4. Choose rotation center (selection center or world origin)

### Symmetry Mode

**Enable Symmetry:**
1. Tools → Symmetry Mode → Enable
2. Choose symmetry axis (X, Y, or Z)
3. All actions are automatically mirrored

**Disable Symmetry:**
- Tools → Symmetry Mode → Disable

**Benefits:**
- Create balanced station designs
- Automatic duplicate placement
- Maintains structural symmetry

### Auto-Arrange

**Grid Arrangement:**
- Tools → Arrange → Grid
- Modules arranged in square grid pattern
- Spacing configurable

**Circle Arrangement:**
- Tools → Arrange → Circle
- Modules arranged in circle
- Radius configurable

**Line Arrangement:**
- Tools → Arrange → Line
- Modules arranged in straight line
- Direction and spacing configurable

---

## Templates

### Using Templates

**Load Template:**
1. File → Load Template
2. Select from available templates
3. Station design loads immediately

**Starter Templates:**
- **Trade Outpost**: Small trading station (3 modules)
- **Mining Station**: Mining operation (5 modules)
- **Research Facility**: Science station (4 modules)
- **Military Base**: Fortified installation (6 modules)
- **Trading Hub**: Large commercial hub (9 modules)

### Creating Templates

**Save as Template:**
1. Design your station
2. File → Save as Template
3. Enter template name and description
4. Template saved to: `Saved/StationTemplates/`

### Managing Templates

**Template Library:**
- File → Template Library
- View all saved templates
- Preview, load, or delete templates

---

## Validation

### Running Validation

**Manual Validation:**
1. Click **Validate** button in toolbar
2. Validation results appear in panel
3. Review errors, warnings, and info messages

**Auto-Validation:**
- Validation runs automatically before export
- Cannot export stations with errors
- Warnings are informational only

### Validation Rules

**Errors (Must Fix):**
- ❌ No docking facility - Add DockingBay or DockingPort
- ❌ No modules - Add at least one module
- ❌ Invalid connections - Check connection compatibility

**Warnings (Recommended):**
- ⚠️ No marketplace - Trading capability limited
- ⚠️ No cargo storage - Limited inventory
- ⚠️ Power deficit - Add reactors or solar arrays
- ⚠️ Disconnected modules - Check connectivity
- ⚠️ Large station (50+ modules) - Performance impact

**Info:**
- ℹ️ Power surplus - Sufficient power generation
- ℹ️ Station statistics - Module count, power balance

---

## Export

### Exporting to Blueprint

**Export Process:**
1. Design station
2. Run validation (automatic before export)
3. Click **Export** button
4. Choose export location
5. Enter Blueprint name
6. Blueprint created in: `/Game/Blueprints/Stations/Generated/`

**Blueprint Contains:**
- Parent class: `ASpaceStation`
- Child Actor Components for each module
- Preserved transforms and connections
- Station metadata

### Using Exported Station

1. Navigate to export location in Content Browser
2. Drag Blueprint into level
3. Station spawns with all modules
4. Fully functional in Adastrea game

---

## Visualization Features

### Power Flow Visualization

**Enable/Disable:**
- View → Show Power Flow

**Visualization:**
- Green lines: Power generation
- Red lines: Power consumption
- Animated flow shows power direction
- Line thickness indicates power amount

### Module Group Colors

**Enable/Disable:**
- View → Show Module Colors

**Color Scheme:**
- Light Blue: Docking
- Yellow: Power
- Brown: Storage
- Orange: Processing
- Red: Defence
- Green: Habitation
- Purple: Public
- Gray: Connection
- Dark Gray: Other

**Customize Colors:**
- View → Customize Colors
- Select module group
- Choose new color

### Connection Wires

**Enable/Disable:**
- View → Show Connection Wires

**Wire Colors:**
- Blue: Valid connections
- Red: Invalid connections
- Yellow: Power connections
- Green: Universal connections

### Traffic Flow (Optional)

**Enable/Disable:**
- View → Show Traffic Flow

**Shows:**
- Docking to marketplace paths
- Cargo movement routes
- NPC traffic simulation

---

## Keyboard Shortcuts

### General
- **Ctrl+N**: New station
- **Ctrl+O**: Open station
- **Ctrl+S**: Save station
- **Ctrl+Shift+S**: Save as template
- **Ctrl+E**: Export to Blueprint

### Editing
- **Ctrl+C**: Copy selected modules
- **Ctrl+V**: Paste modules
- **Ctrl+X**: Cut modules
- **Delete**: Remove selected modules
- **Ctrl+Z**: Undo
- **Ctrl+Y**: Redo

### View
- **F**: Focus on selection
- **W/A/S/D**: Move camera
- **Q/E**: Camera up/down
- **Mouse Wheel**: Zoom
- **Middle Mouse**: Pan camera
- **Right Mouse**: Rotate camera

### Tools
- **Ctrl+M**: Mirror selection
- **Ctrl+R**: Rotate selection
- **Ctrl+G**: Grid arrange
- **Ctrl+Shift+G**: Auto-snap to grid

---

## Tips and Tricks

### Design Best Practices

1. **Start with docking**: Always place docking facility first
2. **Plan power early**: Add reactors before large expansions
3. **Use corridors**: Connect disparate sections
4. **Balance power**: Monitor power consumption vs generation
5. **Test connectivity**: Validate regularly during design

### Performance Tips

1. **Module limit**: Keep under 50 modules for best performance
2. **Connection count**: Limit connections per module to 6-8
3. **Complexity**: Simpler designs load faster
4. **Templates**: Use templates as starting points

### Common Patterns

**Hub and Spoke:**
- Central hub with docking, marketplace
- Radial modules for specialized functions
- Easy navigation, efficient layout

**Linear:**
- Modules in straight line
- Simple to build and navigate
- Good for corridor-heavy designs

**Grid:**
- Organized square/rectangular layout
- Predictable structure
- Easy to expand

**Layered:**
- Multiple parallel planes
- Separated by function (commercial, industrial, residential)
- Vertical organization

---

## Troubleshooting

### Common Issues

**Plugin doesn't appear in menu**
- Verify plugin is enabled in Edit → Plugins
- Restart Unreal Engine editor
- Check plugin loaded successfully in Output Log

**Modules won't connect**
- Check connection types are compatible
- Verify connection sizes match
- Ensure modules are within snap distance (50cm)
- Check connection points aren't already occupied

**Export fails**
- Run validation first
- Fix all error messages
- Ensure export path is valid
- Check write permissions on target directory

**Performance issues**
- Reduce module count (under 50 recommended)
- Disable real-time visualization features
- Close other editor windows
- Increase snap distance to reduce calculations

**Cannot load saved design**
- Check file isn't corrupted
- Verify file format (.stationdesign)
- Ensure all referenced modules exist in project
- Check file version compatibility

### Getting Help

**Resources:**
- Documentation: `/Docs/` folder
- Architecture: `docs/ARCHITECTURE.md`
- Quick Reference: `docs/QUICK_REFERENCE.md`
- GitHub Issues: Report bugs and request features

**Support:**
- Check existing GitHub issues first
- Provide detailed error messages
- Include steps to reproduce
- Attach Output Log if applicable

---

## Appendix

### File Formats

**.stationdesign**: Station design files (JSON format)
**.template**: Template files (JSON format)

### Directories

- **Templates**: `Saved/StationTemplates/`
- **Exports**: `/Game/Blueprints/Stations/Generated/`
- **Config**: `Config/ModularStationDesigner/`

### Version History

- **1.0.0** (2026-01-18): Initial release with all features

---

**End of User Guide**

For technical documentation, see `docs/ARCHITECTURE.md`  
For contributing, see `CONTRIBUTING.md`
