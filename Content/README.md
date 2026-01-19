# Content Directory

This directory contains plugin content assets.

## Structure

- `Icons/` - UI icons for the plugin interface
- `ModuleThumbnails/` - Preview images for station modules
- `EditorAssets/` - Editor-only assets and resources

## Asset Guidelines

### Icons
- Format: PNG with transparency
- Size: 16x16, 20x20, 40x40 pixels (multiple sizes for different UI contexts)
- Style: Flat design, consistent with Unreal Engine editor style
- Naming: `Icon_[Function].png` (e.g., `Icon_Export.png`)

### Module Thumbnails
- Format: PNG or JPG
- Size: 256x256 pixels (will be scaled for display)
- View: Top-down or isometric view of module
- Background: Transparent or neutral dark gray
- Naming: Match module Blueprint name

### Editor Assets
- Miscellaneous editor-only resources
- Visual indicators and gizmos
- Debug visualization assets

## Development Status

📝 Asset creation will begin in Phase 1, Week 3 during UI development.

See [ROADMAP.md](../ROADMAP.md) for the complete development timeline.
