# Segfault
## Build instructions
- Prerequisities:
  - Make sure that the vulkan SDK is installed
  - Make sure that glslc from the SDK is in your PATH
- Checkout the engine code with all submodules:
```
git checkout --recursive https://github.com/kimkulling/Segfault.git
```
- Got to the directory and run cmake:
```
cd SegFault
cmake CMakeLists.txt --preset=default
```
Compile the shader 
```
cd assets/shaders
glsl default.vert -o vert.spv
glsl default.frag -o frag.spv
mkdir ../../bin/debug/shaders
cp *spv ../../bin/debug/shaders
```
- Run the first application on WINdows:
```
cd bin\debug
.\hello_world.exe
```
- Run the first application on Linux:
```
cd bin
./hello_world.exe
```

## Roadmap
### Done 
- Core Concepts
  - Define base architecture
  - Define CMake-Build
  - Windows Management
    - Windows creation
    - Base handling of events

### ToDo
- Core Concepts
  - Rendering
    - Define render thread
      - Define render graph concept
      - Define vulkan renderer
  - Assets
    - Define asset structure
  - Editor
    - Simple UI
- Game 
  - Start -> October
