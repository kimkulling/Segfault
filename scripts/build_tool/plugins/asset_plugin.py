"""
Asset copy plugin for the Segfault build tool.
"""

import os
import shutil
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List
from os import listdir
from os.path import isfile, join

from ..plugin import BuildPlugin, PluginContext, PluginResult
from ..config import BuildConfig


class AssetPlugin(BuildPlugin):
    """Plugin for copying asset files to the build directory."""
    
    _config: Dict[str, Any] = {}
    
    @classmethod
    def get_name(cls) -> str:
        return "asset"
    
    @classmethod
    def get_description(cls) -> str:
        return "Copies asset files (textures, models, etc.) to the build directory"
    
    @classmethod
    def get_dependencies(cls) -> List[str]:
        # Asset copy depends on cmake to know the build directory
        return ["cmake"]
    
    @classmethod
    def get_default_config(cls) -> Dict[str, Any]:
        return {
            "asset_dirs": ["assets/textures", "assets/manifests"],
            "output_dir": None,  # Auto-detected based on build dir
            "file_extensions": None,  # None means copy all files
            "preserve_structure": True  # Preserve subdirectory structure
        }
    
    @classmethod
    def configure(cls, config: Dict[str, Any]) -> bool:
        """Configure the plugin with the given configuration."""
        cls._config = config
        return True
    
    @classmethod
    def _copy_file(cls, source: Path, dest: Path) -> bool:
        """Copy a single file."""
        try:
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, dest)
            return True
        except Exception as e:
            print(f"Error copying {source} to {dest}: {e}")
            return False
    
    @classmethod
    def run(cls, context: PluginContext) -> PluginResult:
        """Copy all assets to the build directory."""
        config: BuildConfig = context.config
        project_root = context.project_root
        build_dir = context.build_dir
        
        # Get plugin configuration
        asset_dirs = cls._config.get('asset_dirs', ["assets/textures", "assets/models", "assets/manifests"])
        output_dir_str = cls._config.get('output_dir')
        output_dir = Path(output_dir_str) if output_dir_str else None
        file_extensions = cls._config.get('file_extensions')
        preserve_structure = cls._config.get('preserve_structure', True)
        
        # Resolve output directory
        if output_dir and not output_dir.is_absolute():
            output_dir = project_root / output_dir
        elif not output_dir:
            # Default: use assets subdirectory in build directory
            output_dir = build_dir / "assets"
        
        total_files_copied = 0
        total_files_failed = 0
        copied_files = []
        
        for asset_dir_str in asset_dirs:
            asset_dir = Path(asset_dir_str)
            
            # Resolve asset directory relative to project root
            if not asset_dir.is_absolute():
                asset_dir = project_root / asset_dir
            
            if not asset_dir.exists():
                print(f"Warning: Asset directory not found: {asset_dir}")
                continue
            
            # Determine destination directory
            if preserve_structure:
                # asset_dir is like "assets/textures", we want "textures" under output_dir
                # Get the path relative to assets
                try:
                    assets_base = project_root / "assets"
                    rel_to_assets = asset_dir.relative_to(assets_base)
                    dest_dir = output_dir / rel_to_assets
                except ValueError:
                    # asset_dir is not under assets, use it as-is
                    rel_path = asset_dir.relative_to(project_root)
                    dest_dir = output_dir / rel_path
            else:
                dest_dir = output_dir
            
            # Find all files in the asset directory
            all_files = []
            for root, dirs, files in os.walk(asset_dir):
                for file in files:
                    file_path = Path(root) / file
                    all_files.append(file_path)
            
            # Filter by extensions if specified
            if file_extensions:
                filtered_files = []
                for file_path in all_files:
                    if file_path.suffix.lower() in file_extensions:
                        filtered_files.append(file_path)
                all_files = filtered_files
            
            # Copy each file
            for source_file in all_files:
                # Calculate relative path within asset directory
                if preserve_structure:
                    rel_to_asset = source_file.relative_to(asset_dir)
                    dest_file = dest_dir / rel_to_asset
                else:
                    dest_file = dest_dir / source_file.name
                
                if cls._copy_file(source_file, dest_file):
                    total_files_copied += 1
                    copied_files.append({
                        "source": str(source_file),
                        "destination": str(dest_file)
                    })
                    if config.verbose:
                        print(f"Copied: {source_file} -> {dest_file}")
                else:
                    total_files_failed += 1
        
        if total_files_failed > 0:
            return PluginResult.failure_result(
                message=f"Copied {total_files_copied} files, {total_files_failed} failed",
                data={
                    "files_copied": total_files_copied,
                    "files_failed": total_files_failed,
                    "copied_files": copied_files
                }
            )
        
        print(f"Copied {total_files_copied} asset file(s) to {output_dir}")
        
        return PluginResult.success_result(
            message=f"Successfully copied {total_files_copied} asset(s)",
            data={
                "files_copied": total_files_copied,
                "files_failed": total_files_failed,
                "copied_files": copied_files
            }
        )
