#!/usr/bin/env python3
"""
Segfault Build Wrapper - Entry point for the CMake-based build system.

This script provides a convenient command-line interface for building the Segfault
game engine. It wraps CMake operations with a plugin-based architecture that
supports shader compilation, asset deployment, and other build tasks.

Usage:
    python segfault [options]
    ./segfault [options]  # On Unix-like systems with execute permission

Quick Start:
    # Build with default settings (Release mode)
    python segfault
    
    # Build in Debug mode
    python segfault --build-type Debug
    
    # Build a specific target
    python segfault --target hello_world
    
    # Clean and rebuild
    python segfault --clean
    
    # Build only (skip shader and asset plugins)
    python segfault --plugins cmake
    
    # Run only shader compilation
    python segfault --plugins shader
    
    # Run only asset deployment
    python segfault --plugins asset
    
    # List available plugins
    python segfault --plugins help
    
    # Use a custom CMake preset
    python segfault --preset my-preset

CMake Settings:
    # Specify CMake generator
    python segfault --generator "Visual Studio 17 2022"
    
    # Pass additional CMake arguments
    python segfault --cmake-args -DCMAKE_BUILD_TESTING=ON
    
    # Disable parallel build
    python segfault --no-parallel
    
    # Specify number of parallel jobs
    python segfault --jobs 4

Configuration File:
    # Use a JSON configuration file
    python segfault --config-file build_config.json

Examples:
    # Full build with Debug configuration
    python segfault --build-type Debug --clean
    
    # Build only the editor target
    python segfault --target segfault_editor
    
    # Build with custom preset and verbose output
    python segfault --preset windows-release --verbose
"""

import os
import sys
from pathlib import Path


# Add the scripts directory to the path so we can import segfault_tool
scripts_dir = Path(__file__).parent / "scripts"
sys.path.insert(0, str(scripts_dir.resolve()))

# Import the main build tool
from segfault_tool.main import main


if __name__ == "__main__":
    # Change to project root directory for consistent relative paths
    project_root = Path(__file__).parent.resolve()
    os.chdir(project_root)
    
    sys.exit(main())
