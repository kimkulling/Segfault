"""
Shader compilation plugin for the Segfault build tool.
"""

import os
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List
from os import listdir
from os.path import isfile, join

from ..plugin import BuildPlugin, PluginContext, PluginResult
from ..config import BuildConfig


class ShaderPlugin(BuildPlugin):
    """Plugin for compiling GLSL shaders to SPIR-V."""
    
    _config: Dict[str, Any] = {}
    
    @classmethod
    def get_name(cls) -> str:
        return "shader"
    
    @classmethod
    def get_description(cls) -> str:
        return "Compiles GLSL shaders to SPIR-V using glslc"
    
    @classmethod
    def get_dependencies(cls) -> List[str]:
        # Shader compilation depends on cmake to know the build directory
        return ["cmake"]
    
    @classmethod
    def get_default_config(cls) -> Dict[str, Any]:
        return {
            "shader_dir": "assets/shaders",
            "output_dir": None,  # Auto-detected based on build dir
            "shader_extensions": [".vert", ".frag", ".comp", ".geom", ".tesc", ".tese"],
            "include_dirs": [],
            "definitions": [],
            "optimization_level": "O",  # O, g, or none
            "generate_debug_info": False
        }
    
    @classmethod
    def configure(cls, config: Dict[str, Any]) -> bool:
        """Configure the plugin with the given configuration."""
        cls._config = config
        return True
    
    @classmethod
    def _compile_shader(
        cls,
        shader_path: Path,
        output_path: Path,
        verbose: bool = False,
        include_dirs: List[str] = None,
        definitions: List[str] = None,
        optimization_level: str = None,
        generate_debug_info: bool = False
    ) -> bool:
        """Compile a single shader file."""
        if not shader_path.exists():
            print(f"Warning: Shader file not found: {shader_path}")
            return False
        
        # Ensure output directory exists
        output_path.parent.mkdir(parents=True, exist_ok=True)
        
        # Build glslc command
        cmd = ["glslc"]
        
        # Note: Optimization level with -O flag requires -c (compile only)
        # but -c with -O generates multiple files. For now, we skip optimization
        # when output is specified. Users can add custom glslc args via include_dirs.
        # if optimization_level and not output_path:
        #     cmd.extend(["-c", "-O", optimization_level])
        
        # Add debug info flag
        if generate_debug_info:
            cmd.append("-g")
        
        # Add include directories
        if include_dirs:
            for inc_dir in include_dirs:
                cmd.extend(["-I", inc_dir])
        
        # Add definitions
        if definitions:
            for definition in definitions:
                cmd.append(f"-D{definition}")
        
        # Input and output
        cmd.append(str(shader_path))
        cmd.append("-o")
        cmd.append(str(output_path))
        
        if verbose:
            print(f"Compiling shader: {' '.join(cmd)}")
        else:
            print(f"Compiling {shader_path.name}...")
        
        # Run glslc
        try:
            result = subprocess.run(
                cmd,
                capture_output=True,
                text=True,
                check=False
            )
            
            if result.returncode != 0:
                print(f"Error compiling {shader_path.name}:")
                if result.stderr:
                    print(result.stderr)
                if verbose and result.stdout:
                    print(result.stdout)
                return False
            
            if verbose and result.stdout:
                print(result.stdout)
            
            print(f"Successfully compiled {shader_path.name}")
            return True
            
        except FileNotFoundError:
            print(f"Error: glslc not found. Please ensure Vulkan SDK is installed and in PATH.")
            return False
        except Exception as e:
            print(f"Error compiling {shader_path.name}: {e}")
            return False
    
    @classmethod
    def _get_output_path(
        cls,
        shader_path: Path,
        build_dir: Path,
        output_dir: Path = None
    ) -> Path:
        """Get the output path for a compiled shader."""
        # Determine output directory
        if output_dir:
            base_dir = output_dir
        else:
            # Default: use shaders subdirectory in build directory
            base_dir = build_dir / "shaders"
        
        # Keep the original filename but change extension to .spv
        # e.g., default.vert -> default.vert.spv, default.frag -> default.frag.spv
        output_name = shader_path.name + ".spv"
        return base_dir / output_name
    
    @classmethod
    def run(cls, context: PluginContext) -> PluginResult:
        """Compile all shaders in the configured directory."""
        config: BuildConfig = context.config
        project_root = context.project_root
        build_dir = context.build_dir
        
        # Get plugin configuration
        shader_dir = Path(cls._config.get('shader_dir', 'assets/shaders'))
        output_dir_str = cls._config.get('output_dir')
        output_dir = Path(output_dir_str) if output_dir_str else None
        shader_extensions = cls._config.get('shader_extensions', [".vert", ".frag", ".comp", ".geom"])
        include_dirs = cls._config.get('include_dirs', [])
        definitions = cls._config.get('definitions', [])
        optimization_level = cls._config.get('optimization_level', 'O')
        generate_debug_info = cls._config.get('generate_debug_info', False)
        verbose = config.verbose
        
        # Resolve shader directory relative to project root
        if not shader_dir.is_absolute():
            shader_dir = project_root / shader_dir
        
        # Resolve output directory relative to project root
        if output_dir and not output_dir.is_absolute():
            output_dir = project_root / output_dir
        
        # Resolve include directories
        resolved_include_dirs = []
        for inc_dir in include_dirs:
            if not os.path.isabs(inc_dir):
                inc_dir = str(project_root / Path(inc_dir))
            resolved_include_dirs.append(inc_dir)
        
        if not shader_dir.exists():
            return PluginResult.failure_result(
                f"Shader directory not found: {shader_dir}"
            )
        
        # Find all shader files
        shader_files = []
        for ext in shader_extensions:
            shader_files.extend(shader_dir.glob(f"*{ext}"))
        
        if not shader_files:
            print(f"No shader files found in {shader_dir} with extensions {shader_extensions}")
            return PluginResult.success_result(
                message="No shaders to compile",
                data={"shaders_compiled": 0}
            )
        
        print(f"Found {len(shader_files)} shader file(s) to compile")
        
        # Compile each shader
        compiled_count = 0
        failed_count = 0
        compiled_shaders = []
        
        for shader_file in shader_files:
            output_path = cls._get_output_path(shader_file, build_dir, output_dir)
            
            if cls._compile_shader(
                shader_path=shader_file,
                output_path=output_path,
                verbose=verbose,
                include_dirs=resolved_include_dirs,
                definitions=definitions,
                optimization_level=optimization_level,
                generate_debug_info=generate_debug_info
            ):
                compiled_count += 1
                compiled_shaders.append({
                    "input": str(shader_file),
                    "output": str(output_path)
                })
            else:
                failed_count += 1
        
        if failed_count > 0:
            return PluginResult.failure_result(
                message=f"Compiled {compiled_count} shaders, {failed_count} failed",
                data={
                    "shaders_compiled": compiled_count,
                    "shaders_failed": failed_count,
                    "compiled_shaders": compiled_shaders
                }
            )
        
        return PluginResult.success_result(
            message=f"Successfully compiled {compiled_count} shader(s)",
            data={
                "shaders_compiled": compiled_count,
                "shaders_failed": failed_count,
                "compiled_shaders": compiled_shaders
            }
        )
