"""
Configuration management for the Segfault build tool.
"""

import argparse
import json
import os
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional


@dataclass
class BuildConfig:
    """Configuration for the build process."""
    
    # Build settings
    build_type: str = "Release"
    build_dir: str = "."
    install_dir: Optional[str] = None
    clean_first: bool = False
    verbose: bool = False
    
    # CMake settings
    cmake_preset: str = "default"
    cmake_generator: Optional[str] = None
    cmake_args: List[str] = field(default_factory=list)
    
    # Plugin settings
    plugins: List[str] = field(default_factory=lambda: ["cmake", "shader", "asset"])
    plugin_configs: Dict[str, Dict[str, Any]] = field(default_factory=dict)
    
    # Target settings
    target: Optional[str] = None
    targets: List[str] = field(default_factory=list)
    
    # Parallel build
    parallel: bool = True
    jobs: Optional[int] = None
    
    @classmethod
    def from_args(cls, args: argparse.Namespace) -> "BuildConfig":
        """Create a BuildConfig from command line arguments."""
        # Parse plugin configs from command line
        plugin_configs: Dict[str, Dict[str, Any]] = {}
        if hasattr(args, 'plugin_config') and args.plugin_config:
            for config in args.plugin_config:
                if '=' in config:
                    # Find the first '=' that's not inside braces
                    # This allows JSON values with '=' characters
                    brace_depth = 0
                    eq_pos = -1
                    for i, c in enumerate(config):
                        if c == '{':
                            brace_depth += 1
                        elif c == '}':
                            brace_depth -= 1
                        elif c == '=' and brace_depth == 0:
                            eq_pos = i
                            break
                    
                    if eq_pos > 0:
                        plugin_name = config[:eq_pos]
                        config_str = config[eq_pos + 1:]
                        try:
                            plugin_configs[plugin_name] = json.loads(config_str)
                        except json.JSONDecodeError:
                            # Treat as simple key-value
                            plugin_configs[plugin_name] = {"value": config_str}
        
        # Parse targets
        targets: List[str] = []
        if args.target:
            targets = args.target.split(',')
        
        # Parse plugins
        plugins = [p.strip() for p in (args.plugins or "cmake,shader,asset").split(',')]
        
        return cls(
            build_type=args.build_type or "Release",
            build_dir=args.build_dir or ".",
            install_dir=args.install_dir,
            clean_first=args.clean,
            verbose=args.verbose,
            cmake_preset=args.preset or "default",
            cmake_generator=args.generator,
            cmake_args=args.cmake_args or [],
            plugins=plugins,
            plugin_configs=plugin_configs,
            target=args.target,
            targets=targets,
            parallel=not args.no_parallel,
            jobs=args.jobs
        )
    
    @classmethod
    def from_file(cls, config_path: str) -> "BuildConfig":
        """Load configuration from a JSON file."""
        with open(config_path, 'r') as f:
            config_data = json.load(f)
        
        return cls(
            build_type=config_data.get('build_type', 'Release'),
            build_dir=config_data.get('build_dir', '.'),
            install_dir=config_data.get('install_dir'),
            clean_first=config_data.get('clean_first', False),
            verbose=config_data.get('verbose', False),
            cmake_preset=config_data.get('cmake_preset', 'default'),
            cmake_generator=config_data.get('cmake_generator'),
            cmake_args=config_data.get('cmake_args', []),
            plugins=config_data.get('plugins', ['cmake', 'shader', 'asset']),
            plugin_configs=config_data.get('plugin_configs', {}),
            target=config_data.get('target'),
            targets=config_data.get('targets', []),
            parallel=config_data.get('parallel', True),
            jobs=config_data.get('jobs')
        )


def create_parser() -> argparse.ArgumentParser:
    """Create the argument parser for the build tool."""
    parser = argparse.ArgumentParser(
        description="Segfault Build Tool - A Python-based build system with plugin support"
    )
    
    # Build settings
    build_group = parser.add_argument_group("Build Settings")
    build_group.add_argument(
        '--build-type', '-t',
        choices=['Debug', 'Release', 'RelWithDebInfo', 'MinSizeRel'],
        default='Release',
        help="Build type (default: Release)"
    )
    build_group.add_argument(
        '--build-dir', '-b',
        default='.',
        help="Build directory (default: current directory)"
    )
    build_group.add_argument(
        '--install-dir',
        help="Installation directory"
    )
    build_group.add_argument(
        '--clean', '-c',
        action='store_true',
        help="Clean before building"
    )
    build_group.add_argument(
        '--verbose', '-v',
        action='store_true',
        help="Verbose output"
    )
    build_group.add_argument(
        '--no-parallel',
        action='store_true',
        help="Disable parallel build"
    )
    build_group.add_argument(
        '--jobs', '-j',
        type=int,
        help="Number of parallel jobs (default: auto)"
    )
    
    # CMake settings
    cmake_group = parser.add_argument_group("CMake Settings")
    cmake_group.add_argument(
        '--preset', '-p',
        default='default',
        help="CMake preset to use (default: default)"
    )
    cmake_group.add_argument(
        '--generator', '-G',
        help="CMake generator"
    )
    cmake_group.add_argument(
        '--cmake-args',
        action='append',
        default=[],
        help="Additional CMake arguments (can be specified multiple times)"
    )
    
    # Target settings
    target_group = parser.add_argument_group("Target Settings")
    target_group.add_argument(
        '--target',
        help="Specific target to build (can be comma-separated list)"
    )
    
    # Plugin settings
    plugin_group = parser.add_argument_group("Plugin Settings")
    plugin_group.add_argument(
        '--plugins',
        default='cmake,shader,asset',
        help="Comma-separated list of plugins to run (default: cmake,shader,asset)"
    )
    plugin_group.add_argument(
        '--plugin-config',
        action='append',
        default=[],
        help="Plugin configuration in format 'plugin_name={\"key\":\"value\"}' (can be specified multiple times)"
    )
    plugin_group.add_argument(
        '--config-file',
        help="Path to JSON configuration file"
    )
    
    return parser


def load_config(args: argparse.Namespace) -> BuildConfig:
    """Load configuration from arguments and config file."""
    # If a config file is specified, load it first
    if args.config_file and os.path.exists(args.config_file):
        config = BuildConfig.from_file(args.config_file)
        # Override with command line arguments
        config.build_type = args.build_type or config.build_type
        config.build_dir = args.build_dir or config.build_dir
        config.install_dir = args.install_dir or config.install_dir
        config.clean_first = args.clean or config.clean_first
        config.verbose = args.verbose or config.verbose
        config.cmake_preset = args.preset or config.cmake_preset
        config.cmake_generator = args.generator or config.cmake_generator
        config.parallel = not args.no_parallel and config.parallel
        config.jobs = args.jobs or config.jobs
        
        # Merge plugin configs
        if hasattr(args, 'plugin_config') and args.plugin_config:
            for config_str in args.plugin_config:
                if '=' in config_str:
                    # Find the first '=' that's not inside braces
                    # This allows JSON values with '=' characters
                    brace_depth = 0
                    eq_pos = -1
                    for i, c in enumerate(config_str):
                        if c == '{':
                            brace_depth += 1
                        elif c == '}':
                            brace_depth -= 1
                        elif c == '=' and brace_depth == 0:
                            eq_pos = i
                            break
                    
                    if eq_pos > 0:
                        plugin_name = config_str[:eq_pos]
                        config_value = config_str[eq_pos + 1:]
                        try:
                            config.plugin_configs[plugin_name] = json.loads(config_value)
                        except json.JSONDecodeError:
                            config.plugin_configs[plugin_name] = {"value": config_value}
        
        # Merge plugins list
        if args.plugins:
            config.plugins = args.plugins.split(',')
        
        return config
    else:
        return BuildConfig.from_args(args)


def get_build_directory(config: BuildConfig) -> Path:
    """Get the absolute build directory path."""
    build_dir = Path(config.build_dir).resolve()
    if config.build_type:
        build_dir = build_dir / config.build_type
    return build_dir
