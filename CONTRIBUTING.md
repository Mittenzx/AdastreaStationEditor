# Contributing to Modular Station Designer

Thank you for your interest in contributing to the Modular Station Designer plugin for Adastrea!

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Pull Request Process](#pull-request-process)
- [Project Structure](#project-structure)

---

## Code of Conduct

### Our Pledge

We are committed to providing a welcoming and inclusive environment for all contributors, regardless of experience level, background, or identity.

### Expected Behavior

- Be respectful and considerate in communication
- Provide constructive feedback
- Focus on what is best for the project and community
- Show empathy towards other contributors

### Unacceptable Behavior

- Harassment, discrimination, or offensive comments
- Personal attacks or trolling
- Publishing others' private information
- Any conduct that would be inappropriate in a professional setting

---

## Getting Started

### Prerequisites

Before contributing, ensure you have:

- **Unreal Engine 5.6** installed (or 5.4+ for development)
- **Visual Studio 2022** (Windows) or **Xcode** (Mac)
- **Git** for version control
- **Adastrea project** (for testing integration)
- Basic knowledge of C++ and Unreal Engine plugin development

### First Time Contributors

If you're new to the project:

1. **Read the documentation**:
   - [README.md](../README.md) - Project overview
   - [ROADMAP.md](../ROADMAP.md) - Development plan
   - [ARCHITECTURE.md](docs/ARCHITECTURE.md) - Technical details

2. **Set up your environment** (see Development Setup below)

3. **Look for "good first issue" labels** on GitHub Issues

4. **Join the discussion** in issue comments before starting work

---

## Development Setup

### 1. Clone the Repository

```bash
git clone https://github.com/Mittenzx/AdastreaStationEditor.git
cd AdastreaStationEditor
```

### 2. Set Up Unreal Engine Plugin

Option A: Development in standalone plugin directory
```bash
# Link plugin to your Unreal Engine plugins directory
# This allows testing in any UE5 project
```

Option B: Development within Adastrea project
```bash
# Clone into Adastrea/Plugins/ directory
cd /path/to/Adastrea/Plugins
git clone https://github.com/Mittenzx/AdastreaStationEditor.git ModularStationDesigner
```

### 3. Build the Plugin

```bash
# Generate project files (Windows)
# Right-click .uproject file and select "Generate Visual Studio project files"

# Open in Visual Studio and build
# Or use command line:
# UnrealBuildTool.exe ModularStationDesigner Win64 Development
```

### 4. Enable Plugin in Unreal Engine

1. Open Unreal Engine editor
2. Edit → Plugins
3. Search for "Modular Station Designer"
4. Enable the plugin
5. Restart editor

### 5. Verify Installation

1. Check that Tools → Station Builder menu item appears
2. Open Station Builder window
3. Verify no errors in Output Log

---

## How to Contribute

### Reporting Bugs

**Before submitting a bug report**:
- Check existing issues to avoid duplicates
- Verify the bug in the latest version
- Gather reproduction steps

**Bug Report Template**:
```markdown
**Describe the bug**
A clear description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '...'
3. See error

**Expected behavior**
What you expected to happen.

**Screenshots**
If applicable, add screenshots.

**Environment**:
- OS: [e.g., Windows 11]
- Unreal Engine Version: [e.g., 5.6.0]
- Plugin Version: [e.g., 0.1.0]

**Additional context**
Any other relevant information.
```

### Suggesting Enhancements

**Before submitting an enhancement**:
- Check if it aligns with the roadmap
- Search existing issues for similar suggestions
- Consider if it fits the project scope

**Enhancement Template**:
```markdown
**Feature Description**
Clear description of the proposed feature.

**Motivation**
Why is this feature needed? What problem does it solve?

**Proposed Solution**
How should this feature work?

**Alternatives Considered**
Other approaches you've thought about.

**Additional Context**
Mockups, examples, or related features.
```

### Contributing Code

1. **Fork the repository** on GitHub

2. **Create a feature branch**:
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b bugfix/bug-description
   ```

3. **Make your changes**:
   - Follow coding standards (see below)
   - Add tests for new functionality
   - Update documentation as needed

4. **Commit your changes**:
   ```bash
   git add .
   git commit -m "Add feature: brief description"
   ```
   
   Commit message format:
   ```
   <type>: <subject>
   
   <body>
   
   <footer>
   ```
   
   Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

5. **Push to your fork**:
   ```bash
   git push origin feature/your-feature-name
   ```

6. **Open a Pull Request** on GitHub

---

## Coding Standards

### C++ Style Guide

We follow the [Unreal Engine C++ Coding Standard](https://docs.unrealengine.com/5.0/en-US/epic-cplusplus-coding-standard-for-unreal-engine/).

**Key Points**:

1. **Naming Conventions**:
   ```cpp
   // Classes: PascalCase with prefix
   class UConnectionPointComponent;
   class FStationValidator;
   class SModulePalette;
   
   // Functions: PascalCase
   void DiscoverModules();
   bool CanConnectTo(UConnectionPointComponent* Other);
   
   // Variables: PascalCase for properties, camelCase for locals
   UPROPERTY()
   FString ModuleName;
   
   float localVariable;
   
   // Booleans: prefix with 'b'
   bool bIsConnected;
   
   // Constants: prefix with 'k' or ALL_CAPS
   const int32 kMaxModules = 100;
   ```

2. **Code Organization**:
   - Header files in `Public/` or `Private/` directories
   - One class per file (generally)
   - Include guards using `#pragma once`
   - Group related functions together

3. **Comments**:
   ```cpp
   /**
    * Brief description of the class.
    * More detailed description if needed.
    */
   class UConnectionPointComponent
   {
       /**
        * Checks if this connection point can connect to another.
        * @param OtherPoint The connection point to check compatibility with
        * @return true if connection is valid, false otherwise
        */
       bool CanConnectTo(UConnectionPointComponent* OtherPoint) const;
   };
   ```

4. **Error Handling**:
   ```cpp
   // Use check() for critical assertions
   check(Module != nullptr);
   
   // Use ensure() for recoverable errors
   if (!ensure(IsValid(Blueprint)))
   {
       return false;
   }
   
   // Use UE_LOG for debugging
   UE_LOG(LogStationDesigner, Warning, TEXT("Failed to load module: %s"), *ModuleName);
   ```

### Blueprint Naming

- Prefix with `BP_`: `BP_StationDesign_TradingHub`
- Use descriptive names: `BP_Module_DockingBay_Large`

### File Organization

```
ModularStationDesigner/
├── Source/
│   ├── ModularStationDesigner/          # Runtime
│   │   ├── Public/                      # Public headers
│   │   │   └── [YourClass].h
│   │   ├── Private/                     # Implementation
│   │   │   └── [YourClass].cpp
│   │   └── ModularStationDesigner.Build.cs
│   └── ModularStationDesignerEditor/    # Editor
│       ├── Public/
│       ├── Private/
│       └── ModularStationDesignerEditor.Build.cs
```

---

## Testing Guidelines

### Unit Tests

Create unit tests for new functionality:

```cpp
// Tests/ConnectionPointTests.cpp
#include "Misc/AutomationTest.h"
#include "ConnectionPoint.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FConnectionPointTest,
    "StationBuilder.ConnectionPoint.BasicConnection",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter
)

bool FConnectionPointTest::RunTest(const FString& Parameters)
{
    // Arrange
    UConnectionPointComponent* PointA = NewObject<UConnectionPointComponent>();
    UConnectionPointComponent* PointB = NewObject<UConnectionPointComponent>();
    
    // Act
    bool bCanConnect = PointA->CanConnectTo(PointB);
    
    // Assert
    TestTrue("Standard connections should be valid", bCanConnect);
    
    return true;
}
```

### Integration Tests

Test end-to-end workflows:
- Module discovery from Adastrea project
- Station design creation and export
- Blueprint asset loading and validation

### Manual Testing Checklist

Before submitting PR:
- [ ] Plugin loads without errors
- [ ] New feature works as expected
- [ ] Existing features not broken
- [ ] No memory leaks (use Unreal Insights)
- [ ] Performance acceptable (60 FPS target)
- [ ] UI responsive and polished

---

## Pull Request Process

### Before Submitting

1. **Update your branch** with latest main:
   ```bash
   git fetch origin
   git rebase origin/main
   ```

2. **Run tests**:
   ```bash
   # Run automated tests in Unreal Engine
   # Editor → Automation → Run Tests
   ```

3. **Check code quality**:
   - No compiler warnings
   - Code follows style guide
   - Comments are clear and helpful

### PR Description Template

```markdown
## Description
Brief description of changes.

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Related Issues
Fixes #[issue number]

## Changes Made
- Added X functionality
- Fixed Y bug
- Updated Z documentation

## Testing
- [ ] Unit tests added/updated
- [ ] Integration tests passed
- [ ] Manual testing completed

## Screenshots (if applicable)
[Add screenshots of UI changes]

## Checklist
- [ ] Code follows project style guide
- [ ] Self-review completed
- [ ] Comments added for complex code
- [ ] Documentation updated
- [ ] No new warnings generated
- [ ] Tests added and passing
```

### Review Process

1. **Automated checks** run on PR submission
2. **Code review** by maintainers
3. **Feedback addressed** by contributor
4. **Approval** from at least one maintainer
5. **Merge** into main branch

### After Merge

- Your contribution will be included in the next release
- You'll be credited in release notes
- Consider contributing to related areas!

---

## Project Structure

### Key Directories

- `Source/ModularStationDesigner/`: Runtime module code
- `Source/ModularStationDesignerEditor/`: Editor module code
- `Content/`: Assets (icons, thumbnails, etc.)
- `Config/`: Plugin configuration files
- `docs/`: Documentation
- `Tests/`: Automated tests (future)

### Key Files

- `ModularStationDesigner.uplugin`: Plugin descriptor
- `README.md`: Project overview
- `ROADMAP.md`: Development timeline
- `CONTRIBUTING.md`: This file
- `docs/ARCHITECTURE.md`: Technical architecture

---

## Getting Help

### Resources

- **Documentation**: Check `docs/` directory
- **Issues**: Browse existing GitHub issues
- **Discussions**: Use GitHub Discussions for questions
- **Adastrea Project**: Reference main Adastrea repository

### Questions?

If you have questions:
1. Check existing documentation
2. Search closed issues for answers
3. Open a new issue with "Question" label
4. Be patient and respectful when asking for help

---

## Recognition

Contributors will be:
- Listed in release notes
- Credited in documentation
- Acknowledged in the project README

Thank you for contributing to the Modular Station Designer!

---

**Document Version**: 1.0  
**Last Updated**: 2026-01-18
