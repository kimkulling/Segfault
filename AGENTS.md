# AGENTS.md - Segfault Game Engine

This file provides guidelines for agentic coding agents working on the Segfault project.

## Project Overview

Segfault is a Vulkan-based game engine written in C++17. The project uses CMake with vcpkg for dependency management.

## Build Commands

### Prerequisites
- Vulkan SDK (with `glslc` in PATH)
- Python 3.11+
- Git (with submodules)

### Initial Setup
```bash
# Clone with submodules
git clone --recursive https://github.com/kimkulling/Segfault.git
cd Segfault

# Configure with CMake preset
cmake --preset=default

# Build the project
cmake --build . --config Release  # or Debug
```

### Build Details
- CMake preset: `default` (uses vcpkg toolchain)
- Output directories:
  - Binaries: `bin/`
  - Libraries: `lib/`
- Build configuration: Release or Debug

### Running Applications
```bash
# Hello World example
cd bin
./hello_world.exe
```

### Shader Compilation
```bash
cd scripts
python compile_shader.py
```

### Running Tests

The project uses Google Test (gtest) for unit tests located in `src/contrib/cppcore/test/`.

#### Build Tests
Tests are built when `CPPCORE_BUILD_UNITTESTS` is ON (default):
```bash
cmake --preset=default -DCPPCORE_BUILD_UNITTESTS=ON
cmake --build .
```

#### Run All Tests
```bash
./bin/cppcore_unittest
```

#### Run Single Test
Use gtest filter to run specific tests:
```bash
./bin/cppcore_unittest --gtest_filter=TArrayTest.constructTest
./bin/cppcore_unittest --gtest_filter=*String*  # All tests matching pattern
```

---

## Code Style Guidelines

### General Conventions

- **C++ Standard**: C++17
- **Header Format**: Use `#pragma once` instead of include guards
- **License Header**: Include MIT license header in all source files:
```cpp
/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling
...
-----------------------------------------------------------------------------------------------*/
```

### Formatting (LLVM-based)

The project uses `.clang-format` (from `src/contrib/cppcore/.clang-format`). Key settings:

| Setting | Value |
|---------|-------|
| BasedOnStyle | LLVM |
| IndentWidth | 4 |
| TabWidth | 4 |
| UseTab | Never |
| ColumnLimit | 0 (unlimited) |
| AccessModifierOffset | -4 |
| AllowShortFunctionsOnASingleLine | Inline |
| AllowShortIfStatementsOnASingleLine | true |
| BreakConstructorInitializers | AfterColon |

Run formatting:
```bash
clang-format -i src/runtime/core/myfile.cpp
```

### Naming Conventions

| Element | Convention | Example |
|---------|------------|---------|
| Classes | PascalCase | `ArgumentParser`, `RenderThread` |
| Functions | PascalCase | `getNext()`, `init()` |
| Member Variables | `m` + PascalCase | `mCurrentIndex`, `mArgument` |
| Constants | `k` + PascalCase | `kMaxSize` |
| Enums | PascalCase | `ModuleState`, `LogType` |
| Enum Values | PascalCase | `ModuleState::Init` |
| Typedefs | PascalCase | `StringArray` |
| Files | PascalCase | `ArgumentParser.h`, `app.cpp` |

### Namespace Usage

All code lives in the `segfault` namespace with sub-namespaces:
```cpp
namespace segfault::core {
    // core module code
}

namespace segfault::renderer {
    // renderer module code
}

namespace segfault::application {
    // application code
}
```

### Import Style

- **Local project headers**: Use quotes
  ```cpp
  #include "core/argumentparser.h"
  #include "application/app.h"
  ```

- **Library headers**: Use angle brackets
  ```cpp
  #include <cppcore/Container/TArray.h>
  #include <nlohmann/json.hpp>
  ```

- **Standard library**: Use angle brackets
  ```cpp
  #include <string>
  #include <vector>
  ```

### Class Design

1. **Virtual destructors** for base classes
2. **Delete copy/move** where appropriate:
   ```cpp
   // No copying
   App(const App& rhs) = delete;
   App& operator=(const App& rhs) = delete;
   ```
3. **Use `final`** for classes not intended for inheritance
4. **Group access**: public, protected, private (in that order)
5. **Member initialization**: Use member initializer lists in constructors

### Error Handling

- **Return bool** for functions that can fail
- **Use optional/result types** from cppcore for complex error states
- Avoid exceptions (not currently used in codebase)

### Documentation

Use Doxygen-style comments in headers:
```cpp
/// @brief Brief description.
/// @param arg1 Description of first parameter.
/// @return Description of return value.
bool myFunction(int arg1);
```

### Platform-Specific Code

Use defined macros:
- `SEGFAULT_WINDOWS` - Windows platform
- `SEGFAULT_GNU_LINUX` - Linux platform

```cpp
#ifdef SEGFAULT_WINDOWS
    // Windows-specific code
#endif
```

### Export Macros

Use `SEGFAULT_EXPORT` for classes/functions that need to be exported from shared libraries:
```cpp
class SEGFAULT_EXPORT App final {
    // ...
};
```

---

## Project Structure

```
src/
├── runtime/           # Core engine library
│   ├── core/         # Core utilities (argparser, tokenizer, etc.)
│   ├── renderer/     # Vulkan rendering
│   └── application/ # Application framework
├── contrib/
│   └── cppcore/     # Common utilities library (containers, memory, etc.)
├── examples/         # Example applications
├── tools/            # Development tools (assetbaker)
└── editor/           # Editor application
assets/
├── shaders/          # GLSL shaders
└── manifests/        # Asset manifests
scripts/             # Build scripts
```

---

## Common Development Tasks

### Adding a New Module
1. Create header/source files in appropriate `src/runtime/` subdirectory
2. Add files to the relevant `CMakeLists.txt`
3. Export classes with `SEGFAULT_EXPORT` if needed

### Adding a New Test
1. Add test file to `src/contrib/cppcore/test/<category>/`
2. Add to corresponding `cppcore_*_test_src` in `CMakeLists.txt`
3. Rebuild and run with `--gtest_filter=TestName.*`

### Running Lint
Currently no automated linting. Use `clang-format` for code formatting.
