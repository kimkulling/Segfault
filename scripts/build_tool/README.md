# Segfault Build Tool

A Python-based build system for the Segfault game engine with extensible plugin support.

## Features

- **CMake Integration**: Configure and build the project using CMake presets
- **Shader Compilation**: Automatically compile GLSL shaders to SPIR-V
- **Asset Management**: Copy asset files to the build directory
- **Plugin System**: Extensible architecture for adding new build steps
- **Configuration**: JSON-based configuration with command-line overrides

## Quick Start

### Basic Build

```bash
# Build with default settings (Release)
python scripts/build.py

# Build in Debug mode
python scripts/build.py --build-type Debug
```

### Common Commands

```bash
# Clean and rebuild
python scripts/build.py --clean

# Build specific target
python scripts/build.py --target hello_world

# Verbose output
python scripts/build.py --verbose

# Single-threaded build
python scripts/build.py --no-parallel

# Specify number of parallel jobs
python scripts/build.py --jobs 4
```

### Plugin Management

```bash
# List available plugins
python scripts/build.py --plugins help

# Run only CMake
python scripts/build.py --plugins cmake

# Run only shader compilation
python scripts/build.py --plugins shader

# Run CMake and shader compilation
python scripts/build.py --plugins cmake,shader

# Configure a plugin
python scripts/build.py --plugin-config 'shader={"optimization_level": "g"}'
```

### Configuration File

```bash
# Use a configuration file
python scripts/build.py --config-file build_config.json
```

## Project Structure

```
scripts/
├── build.py                 # Main entry point (wrapper)
└── build_tool/
    ├── __init__.py
    ├── main.py              # Main execution logic
    ├── config.py            # Configuration management
    ├── plugin.py            # Plugin system base classes
    └── plugins/
        ├── __init__.py
        ├── cmake_plugin.py   # CMake build plugin
        ├── shader_plugin.py # Shader compilation plugin
        └── asset_plugin.py  # Asset copy plugin
```

## Configuration

### Command Line Arguments

#### Build Settings

| Argument | Short | Description | Default |
|----------|-------|-------------|---------|
| `--build-type` | `-t` | Build type (Debug, Release, RelWithDebInfo, MinSizeRel) | Release |
| `--build-dir` | `-b` | Build directory | `.` |
| `--install-dir` | | Installation directory | None |
| `--clean` | `-c` | Clean before building | False |
| `--verbose` | `-v` | Verbose output | False |
| `--no-parallel` | | Disable parallel build | False |
| `--jobs` | `-j` | Number of parallel jobs | Auto |

#### CMake Settings

| Argument | Short | Description | Default |
|----------|-------|-------------|---------|
| `--preset` | `-p` | CMake preset to use | default |
| `--generator` | `-G` | CMake generator | None |
| `--cmake-args` | | Additional CMake arguments (can be specified multiple times) | [] |

#### Target Settings

| Argument | Description | Default |
|----------|-------------|---------|
| `--target` | Specific target to build (comma-separated) | None |

#### Plugin Settings

| Argument | Description | Default |
|----------|-------------|---------|
| `--plugins` | Comma-separated list of plugins to run | cmake,shader,asset |
| `--plugin-config` | Plugin configuration as JSON (can be specified multiple times) | {} |
| `--config-file` | Path to JSON configuration file | None |

### JSON Configuration File

Create a `build_config.json` file:

```json
{
    "build_type": "Release",
    "build_dir": ".",
    "clean_first": false,
    "verbose": false,
    "parallel": true,
    "jobs": null,
    
    "cmake_preset": "default",
    "cmake_generator": null,
    "cmake_args": [],
    
    "plugins": ["cmake", "shader", "asset"],
    "plugin_configs": {
        "cmake": {
            "preset": "default",
            "configure_only": false,
            "build_only": false
        },
        "shader": {
            "shader_dir": "assets/shaders",
            "output_dir": null,
            "shader_extensions": [".vert", ".frag", ".comp", ".geom"],
            "include_dirs": [],
            "definitions": [],
            "optimization_level": "O",
            "generate_debug_info": false
        },
        "asset": {
            "asset_dirs": ["assets/textures", "assets/models", "assets/manifests"],
            "output_dir": null,
            "file_extensions": null,
            "preserve_structure": true
        }
    }
}
```

## Creating New Plugins

To create a new plugin, follow these steps:

1. Create a new file in `scripts/build_tool/plugins/` (e.g., `my_plugin.py`)
2. Implement the `BuildPlugin` abstract base class
3. The plugin will be automatically discovered and registered

### Plugin Template

```python
from build_tool.plugin import BuildPlugin, PluginContext, PluginResult
from typing import Any, Dict, List


class MyPlugin(BuildPlugin):
    """My custom build plugin."""
    
    @classmethod
    def get_name(cls) -> str:
        return "my_plugin"
    
    @classmethod
    def get_description(cls) -> str:
        return "Description of what this plugin does"
    
    @classmethod
    def get_dependencies(cls) -> List[str]:
        # List any plugin dependencies
        return ["cmake"]
    
    @classmethod
    def get_default_config(cls) -> Dict[str, Any]:
        return {
            "option1": "default_value",
            "option2": 42
        }
    
    @classmethod
    def configure(cls, config: Dict[str, Any]) -> bool:
        """Validate and store plugin configuration."""
        cls._config = config
        return True
    
    @classmethod
    def run(cls, context: PluginContext) -> PluginResult:
        """Execute the plugin logic."""
        config = context.config
        project_root = context.project_root
        build_dir = context.build_dir
        
        # Your plugin logic here
        
        return PluginResult.success_result(
            message="Plugin completed successfully",
            data={"key": "value"}
        )
```

### Plugin Configuration

Plugins can be configured via:

1. **Default configuration**: Defined in `get_default_config()`
2. **Global configuration file**: Via the `plugin_configs` section
3. **Command line**: Using `--plugin-config` argument

### Plugin Dependencies

The plugin system automatically resolves dependencies using topological sorting. If plugin A depends on plugin B, specify it in `get_dependencies()`:

```python
@classmethod
def get_dependencies(cls) -> List[str]:
    return ["cmake", "shader"]  # This plugin requires cmake and shader to run first
```

## Built-in Plugins

### CMake Plugin

Configures and builds the project using CMake.

**Configuration Options:**
- `preset`: CMake preset name (default: "default")
- `generator`: CMake generator (default: None)
- `configure_only`: Only run configuration, skip build (default: False)
- `build_only`: Skip configuration, only run build (default: False)
- `target`: Specific target to build (default: None)
- `clean_first`: Clean before building (default: False)

### Shader Plugin

Compiles GLSL shaders to SPIR-V using `glslc`.

**Configuration Options:**
- `shader_dir`: Directory containing shader files (default: "assets/shaders")
- `output_dir`: Output directory for compiled shaders (default: build_dir/shaders)
- `shader_extensions`: File extensions to compile (default: [".vert", ".frag", ".comp", ".geom", ".tesc", ".tese"])
- `include_dirs`: Additional include directories
- `definitions`: Preprocessor definitions
- `optimization_level`: Optimization level ("O", "g", or None)
- `generate_debug_info`: Generate debug information (default: False)

**Requirements:**
- Vulkan SDK with `glslc` in PATH

### Asset Plugin

Copies asset files to the build directory.

**Configuration Options:**
- `asset_dirs`: Directories containing assets (default: ["assets/textures", "assets/models", "assets/manifests"])
- `output_dir`: Output directory for assets (default: build_dir/assets)
- `file_extensions`: File extensions to copy (default: None = all files)
- `preserve_structure`: Preserve directory structure (default: True)

## Examples

### Example 1: Debug Build with Clean

```bash
python scripts/build.py --build-type Debug --clean
```

### Example 2: Build Specific Target

```bash
python scripts/build.py --target hello_world
```

### Example 3: Configure Shader Plugin

```bash
python scripts/build.py --plugin-config 'shader={"optimization_level": "g", "generate_debug_info": true}'
```

### Example 4: Only Compile Shaders

```bash
python scripts/build.py --plugins shader --plugin-config 'shader={"shader_dir": "custom/shaders"}'
```

### Example 5: Custom Configuration File

```bash
# Create custom_config.json with your settings
python scripts/build.py --config-file custom_config.json
```

## Integration with CI/CD

The build tool can be easily integrated into CI/CD pipelines:

```yaml
# GitHub Actions example
- name: Build project
  run: |
    python scripts/build.py --build-type Release
    
- name: Build with tests
  run: |
    python scripts/build.py --build-type Debug --target cppcore_unittest
```

## Troubleshooting

### "No plugins found" Error

Ensure you're running the script from the project root or that the `scripts` directory is in your Python path.

### "glslc not found" Error

Install the Vulkan SDK and ensure `glslc` is in your PATH.

### CMake Configuration Errors

Check that the specified CMake preset exists in `CMakePresets.json`.

### Plugin Dependency Errors

If you see dependency errors, ensure all required plugins are specified in the `--plugins` argument.

## License

MIT License - See the main project LICENSE file for details.
