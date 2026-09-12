"""
CMake build plugin for the Segfault build tool.
"""

import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List

from ..plugin import BuildPlugin, PluginContext, PluginResult
from ..config import BuildConfig


class CMakePlugin(BuildPlugin):
    """Plugin for configuring and building with CMake."""
    
    _config: Dict[str, Any] = {}
    
    @classmethod
    def get_name(cls) -> str:
        return "cmake"
    
    @classmethod
    def get_description(cls) -> str:
        return "Configures and builds the project using CMake"
    
    @classmethod
    def get_dependencies(cls) -> List[str]:
        return []
    
    @classmethod
    def get_default_config(cls) -> Dict[str, Any]:
        return {
            "preset": "default",
            "generator": None,
            "configure_only": False,
            "build_only": False,
            "target": None,
            "clean_first": False
        }
    
    @classmethod
    def configure(cls, config: Dict[str, Any]) -> bool:
        """Configure the plugin with the given configuration."""
        cls._config = config
        return True
    
    @classmethod
    def run(cls, context: PluginContext) -> PluginResult:
        """Execute the CMake build process."""
        config: BuildConfig = context.config
        project_root = context.project_root
        build_dir = context.build_dir
        
        # Create build directory if it doesn't exist
        build_dir.mkdir(parents=True, exist_ok=True)
        
        # Get configuration from context
        preset = cls._config.get('preset') or config.cmake_preset
        generator = cls._config.get('generator') or config.cmake_generator
        configure_only = cls._config.get('configure_only', False)
        build_only = cls._config.get('build_only', False)
        clean_first = cls._config.get('clean_first', False) or config.clean_first
        target = cls._config.get('target') or config.target
        
        # Build CMake configure command
        cmake_cmd = ["cmake"]
        
        if generator:
            cmake_cmd.extend(["-G", generator])
        
        cmake_cmd.extend([
            "--preset", preset,
            f"-B{build_dir}",
            f"-S{project_root}"
        ])
        
        # Add additional CMake arguments
        cmake_cmd.extend(config.cmake_args)
        
        # Add build type
        cmake_cmd.extend([
            f"-DCMAKE_BUILD_TYPE={config.build_type}"
        ])
        
        if config.verbose:
            print(f"CMake configure command: {' '.join(cmake_cmd)}")
        
        # Run configure
        if not build_only:
            print("Configuring project with CMake...")
            result = subprocess.run(
                cmake_cmd,
                cwd=project_root,
                capture_output=True,
                text=True
            )
            
            if result.returncode != 0:
                error_msg = result.stderr or result.stdout
                return PluginResult.failure_result(
                    f"CMake configuration failed with exit code {result.returncode}: {error_msg}"
                )
            
            if config.verbose:
                print(result.stdout)
                if result.stderr:
                    print(result.stderr)
            
            print("CMake configuration completed successfully")
        
        # Run clean if requested
        if clean_first:
            print("Cleaning build directory...")
            clean_cmd = ["cmake", "--build", str(build_dir), "--clean-first"]
            if config.parallel and not config.jobs:
                # Auto-detect parallel jobs
                clean_cmd.append("--parallel")
            elif config.jobs:
                clean_cmd.extend(["--parallel", str(config.jobs)])
            
            result = subprocess.run(
                clean_cmd,
                cwd=build_dir,
                capture_output=True,
                text=True
            )
            
            if result.returncode != 0:
                error_msg = result.stderr or result.stdout
                return PluginResult.failure_result(
                    f"CMake clean failed with exit code {result.returncode}: {error_msg}"
                )
        
        # Run build
        if not configure_only:
            print("Building project...")
            build_cmd = ["cmake", "--build", str(build_dir)]
            
            if config.parallel and not config.jobs:
                build_cmd.append("--parallel")
            elif config.jobs:
                build_cmd.extend(["--parallel", str(config.jobs)])
            
            # Add build type if not already set
            build_cmd.extend(["--config", config.build_type])
            
            if target:
                build_cmd.extend(["--target", target])
            
            if config.verbose:
                print(f"CMake build command: {' '.join(build_cmd)}")
            
            result = subprocess.run(
                build_cmd,
                cwd=build_dir,
                capture_output=True,
                text=True
            )
            
            if result.returncode != 0:
                error_msg = result.stderr or result.stdout
                return PluginResult.failure_result(
                    f"CMake build failed with exit code {result.returncode}: {error_msg}"
                )
            
            if config.verbose:
                print(result.stdout)
                if result.stderr:
                    print(result.stderr)
            
            print("Build completed successfully")
        
        return PluginResult.success_result(
            message="CMake build completed successfully",
            data={
                "build_dir": str(build_dir),
                "build_type": config.build_type
            }
        )
