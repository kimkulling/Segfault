"""
Plugin system for the Segfault build tool.
"""

import abc
import importlib
import inspect
import os
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional, Type

from .config import BuildConfig


@dataclass
class PluginContext:
    """Context passed to plugins during execution."""
    
    config: BuildConfig
    project_root: Path
    build_dir: Path
    
    # Runtime state
    plugin_results: Dict[str, Any] = None
    
    def __post_init__(self):
        if self.plugin_results is None:
            self.plugin_results = {}
    
    def set_result(self, plugin_name: str, result: Any) -> None:
        """Set a result from a plugin that can be used by other plugins."""
        self.plugin_results[plugin_name] = result
    
    def get_result(self, plugin_name: str, default: Any = None) -> Any:
        """Get a result from another plugin."""
        return self.plugin_results.get(plugin_name, default)


@dataclass
class PluginResult:
    """Result of a plugin execution."""
    
    success: bool
    message: str = ""
    data: Any = None
    
    @classmethod
    def success_result(cls, message: str = "", data: Any = None) -> "PluginResult":
        """Create a successful result."""
        return cls(success=True, message=message, data=data)
    
    @classmethod
    def failure_result(cls, message: str = "", data: Any = None) -> "PluginResult":
        """Create a failure result."""
        return cls(success=False, message=message, data=data)


class BuildPlugin(abc.ABC):
    """Base class for all build plugins."""
    
    @classmethod
    @abc.abstractmethod
    def get_name(cls) -> str:
        """Get the unique name of this plugin."""
        pass
    
    @classmethod
    @abc.abstractmethod
    def get_description(cls) -> str:
        """Get a description of what this plugin does."""
        pass
    
    @classmethod
    @abc.abstractmethod
    def get_dependencies(cls) -> List[str]:
        """Get list of plugin names this plugin depends on."""
        pass
    
    @classmethod
    @abc.abstractmethod
    def get_default_config(cls) -> Dict[str, Any]:
        """Get the default configuration for this plugin."""
        pass
    
    @classmethod
    @abc.abstractmethod
    def configure(cls, config: Dict[str, Any]) -> bool:
        """Configure the plugin with the given configuration.
        
        Returns True if configuration is valid, False otherwise.
        """
        pass
    
    @classmethod
    @abc.abstractmethod
    def run(cls, context: PluginContext) -> PluginResult:
        """Execute the plugin.
        
        Args:
            context: The plugin context containing configuration and state.
            
        Returns:
            PluginResult indicating success or failure.
        """
        pass


class PluginManager:
    """Manages the loading and execution of plugins."""
    
    def __init__(self):
        self._plugins: Dict[str, Type[BuildPlugin]] = {}
        self._plugin_instances: Dict[str, BuildPlugin] = {}
    
    def register_plugin(self, plugin_class: Type[BuildPlugin]) -> None:
        """Register a plugin class."""
        plugin_name = plugin_class.get_name()
        self._plugins[plugin_name] = plugin_class
    
    def discover_plugins(self, module_name: str = "segfault_tool.plugins") -> None:
        """Discover and register all plugins in a module."""
        try:
            module = importlib.import_module(module_name)
            module_path = Path(module.__file__).parent
            
            # Import all Python files in the plugins directory
            for file in module_path.glob("*.py"):
                if file.name.startswith("_"):
                    continue
                    
                module_name = f"{module.__name__}.{file.stem}"
                try:
                    plugin_module = importlib.import_module(module_name)
                    
                    # Find all BuildPlugin subclasses
                    for name, obj in inspect.getmembers(plugin_module, inspect.isclass):
                        if issubclass(obj, BuildPlugin) and obj != BuildPlugin:
                            self.register_plugin(obj)
                except ImportError as e:
                    print(f"Warning: Could not import plugin module {module_name}: {e}")
        except ImportError as e:
            print(f"Warning: Could not import plugin module {module_name}: {e}")
    
    def get_plugin(self, name: str) -> Optional[Type[BuildPlugin]]:
        """Get a registered plugin by name."""
        return self._plugins.get(name)
    
    def get_available_plugins(self) -> List[str]:
        """Get list of all available plugin names."""
        return list(self._plugins.keys())
    
    def get_plugin_info(self, name: str) -> Optional[Dict[str, Any]]:
        """Get information about a plugin."""
        plugin_class = self._plugins.get(name)
        if not plugin_class:
            return None
        
        return {
            "name": plugin_class.get_name(),
            "description": plugin_class.get_description(),
            "dependencies": plugin_class.get_dependencies(),
            "default_config": plugin_class.get_default_config()
        }
    
    def resolve_plugin_order(self, plugin_names: List[str]) -> List[str]:
        """Resolve the execution order of plugins based on dependencies.
        
        Args:
            plugin_names: List of plugin names to execute.
            
        Returns:
            List of plugin names in execution order.
            
        Raises:
            ValueError: If there are circular dependencies or missing plugins.
        """
        # Check for missing plugins
        for name in plugin_names:
            if name not in self._plugins:
                raise ValueError(f"Plugin '{name}' is not registered")
        
        # Build dependency graph
        graph: Dict[str, List[str]] = {}
        in_degree: Dict[str, int] = {}
        
        for name in plugin_names:
            plugin_class = self._plugins[name]
            deps = plugin_class.get_dependencies()
            graph[name] = [d for d in deps if d in plugin_names]
            in_degree[name] = 0
        
        # Calculate in-degrees
        for name in plugin_names:
            for dep in graph[name]:
                in_degree[dep] += 1
        
        # Topological sort using Kahn's algorithm
        queue = [name for name in plugin_names if in_degree[name] == 0]
        result: List[str] = []
        
        while queue:
            # Sort for deterministic order
            queue.sort()
            node = queue.pop(0)
            result.append(node)
            
            for neighbor in plugin_names:
                if node in graph[neighbor]:
                    in_degree[neighbor] -= 1
                    if in_degree[neighbor] == 0:
                        queue.append(neighbor)
        
        if len(result) != len(plugin_names):
            raise ValueError("Circular dependency detected in plugins")
        
        return result
    
    def execute_plugins(
        self,
        plugin_names: List[str],
        context: PluginContext
    ) -> Dict[str, PluginResult]:
        """Execute a list of plugins in the correct order.
        
        Args:
            plugin_names: List of plugin names to execute.
            context: The plugin context.
            
        Returns:
            Dictionary mapping plugin names to their results.
        """
        results: Dict[str, PluginResult] = {}
        
        # Resolve execution order
        try:
            execution_order = self.resolve_plugin_order(plugin_names)
        except ValueError as e:
            print(f"Error resolving plugin order: {e}")
            return results
        
        # Execute plugins in order
        for plugin_name in execution_order:
            plugin_class = self._plugins[plugin_name]
            
            # Get plugin-specific config
            plugin_config = context.config.plugin_configs.get(plugin_name, {})
            
            # Merge with default config
            default_config = plugin_class.get_default_config()
            merged_config = {**default_config, **plugin_config}
            
            # Configure the plugin
            if not plugin_class.configure(merged_config):
                print(f"Warning: Invalid configuration for plugin '{plugin_name}'")
            
            # Execute the plugin
            print(f"Running plugin: {plugin_name}")
            result = plugin_class.run(context)
            results[plugin_name] = result
            
            if not result.success:
                print(f"Plugin '{plugin_name}' failed: {result.message}")
                # Continue with other plugins but mark overall failure
                # Could also stop here if desired
            else:
                print(f"Plugin '{plugin_name}' completed successfully")
                
            # Store result in context
            context.set_result(plugin_name, result)
        
        return results


def get_plugin_manager() -> PluginManager:
    """Get a plugin manager with all standard plugins registered."""
    manager = PluginManager()
    manager.discover_plugins()
    return manager
