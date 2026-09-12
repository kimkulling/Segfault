#!/usr/bin/env python3
"""
Segfault Build Tool - Simple wrapper script.

This script provides a convenient way to run the build tool.
Usage:
    python scripts/build.py [options]
    python scripts/build.py --help

Examples:
    # Build with default settings (Release)
    python scripts/build.py
    
    # Build in Debug mode
    python scripts/build.py --build-type Debug
    
    # Build and run only CMake
    python scripts/build.py --plugins cmake
    
    # Build with clean first
    python scripts/build.py --clean
    
    # Run only shader compilation
    python scripts/build.py --plugins shader
    
    # Build with custom preset
    python scripts/build.py --preset my-preset
    
    # Build specific target
    python scripts/build.py --target hello_world
    
    # List available plugins
    python scripts/build.py --plugins help
"""

import os
import sys

# Add the build_tool directory to the path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from build_tool.main import main

if __name__ == "__main__":
    sys.exit(main())
