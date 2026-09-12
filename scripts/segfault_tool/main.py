"""
Main entry point for the Segfault build tool.
"""

import os
import sys
from pathlib import Path
from typing import Any, Dict, List

from .config import create_parser, load_config, get_build_directory, BuildConfig
from .plugin import PluginManager, PluginContext, PluginResult, get_plugin_manager


def print_header():
    """Print the build tool header."""
    print("=" * 60)
    print("Segfault Build Tool")
    print("=" * 60)
    print()


def print_available_plugins(manager: PluginManager):
    """Print information about available plugins."""
    print("Available plugins:")
    print("-" * 40)
    
    for plugin_name in manager.get_available_plugins():
        info = manager.get_plugin_info(plugin_name)
        if info:
            print(f"  {plugin_name}:")
            print(f"    Description: {info['description']}")
            if info['dependencies']:
                print(f"    Dependencies: {', '.join(info['dependencies'])}")
            if info['default_config']:
                print(f"    Config: {info['default_config']}")
            print()


def print_summary(results: Dict[str, PluginResult]):
    """Print a summary of plugin execution results."""
    print("=" * 60)
    print("Build Summary")
    print("=" * 60)
    
    success_count = sum(1 for r in results.values() if r.success)
    failure_count = len(results) - success_count
    
    print(f"Total plugins executed: {len(results)}")
    print(f"Successful: {success_count}")
    print(f"Failed: {failure_count}")
    print()
    
    for plugin_name, result in results.items():
        status = "SUCCESS" if result.success else "FAILED"
        print(f"  [{status}] {plugin_name}: {result.message}")
    
    print()


def main():
    """Main function for the build tool."""
    print_header()
    
    # Get project root
    project_root = Path(__file__).parent.parent.parent.resolve()
    
    # Parse arguments
    parser = create_parser()
    args = parser.parse_args()
    
    # Handle help for plugins
    if args.plugins == "help":
        manager = get_plugin_manager()
        print_available_plugins(manager)
        return 0
    
    # Load configuration
    try:
        config = load_config(args)
    except Exception as e:
        print(f"Error loading configuration: {e}")
        return 1
    
    # Get build directory
    build_dir = get_build_directory(config)
    
    # Initialize plugin manager
    manager = get_plugin_manager()
    
    # Check if plugins exist
    if not manager.get_available_plugins():
        print("Error: No plugins found. Please ensure plugins are installed.")
        return 1
    
    # Parse plugin list
    plugins_to_run = [p.strip() for p in config.plugins]
    
    # Validate plugins
    available_plugins = manager.get_available_plugins()
    invalid_plugins = [p for p in plugins_to_run if p not in available_plugins]
    
    if invalid_plugins:
        print(f"Error: Unknown plugin(s): {', '.join(invalid_plugins)}")
        print(f"Available plugins: {', '.join(available_plugins)}")
        return 1
    
    # Check for empty plugin list
    if not plugins_to_run:
        print("Error: No plugins specified")
        return 1
    
    print(f"Project root: {project_root}")
    print(f"Build directory: {build_dir}")
    print(f"Build type: {config.build_type}")
    print(f"Plugins to run: {', '.join(plugins_to_run)}")
    print()
    
    # Create plugin context
    context = PluginContext(
        config=config,
        project_root=project_root,
        build_dir=build_dir
    )
    
    # Execute plugins
    results = manager.execute_plugins(plugins_to_run, context)
    
    # Print summary
    print_summary(results)
    
    # Check for failures
    failures = [r for r in results.values() if not r.success]
    if failures:
        print("Build completed with errors.")
        return 1
    else:
        print("Build completed successfully!")
        return 0


if __name__ == "__main__":
    sys.exit(main())
