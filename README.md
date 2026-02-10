<p align="center">
  <img src="https://github.com/kimkulling/Segfault/blob/main/assets/SegFault.jpg" />
</p>
# Segfault
## Build status
[![CMake on multiple platforms](https://github.com/kimkulling/Segfault/actions/workflows/cmake-multi-platform.yml/badge.svg)]

## Build instructions
- **Prerequisities:**
  - Vulkan SDK
  - PYthon 3.11
  - Make sure that glslc from the SDK is in your PATH
- **Checkout the engine code with all submodules:**
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
cd scripts
python compile_shader.py
```
- Run the first application on Windows:
```
cd bin\<config>
.\hello_world.exe
```
- Run the first application on Linux:
```
cd bin
./hello_world.exe
```

## Roadmap for Version 0.0.1
- [ ] Core Concepts 
  - [ ] Define base architecture
  - [x] Define CMake-Build
  - [x] Windows Management
    - [x] Windows creation
    - [x] Base handling of events
- [ ] Core Concepts
  - [ ] Rendering
    - [x] Define vulkan renderer
      - [x] Base concept
      - [ ] Textures
      - [ ] Model loading
    - [ ] Define render thread
      - [ ] Define render graph concept
  - [ ] Assets
    - [ ] Define asset structure
  - [ ] Editor
    - [ ] Simple UI
- [ ] Game
  - [ ] Start with Pong -> January 2026
