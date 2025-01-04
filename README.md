# Engine2D

[![CMake on multiple platforms](https://github.com/olesgedz/2dRendererLua/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=master)](https://github.com/olesgedz/2dRendererLua/actions/workflows/cmake-multi-platform.yml)

2D rendering engine with ECS

Completed features:

- ECS System
- Animation system
- Events
- Unit tests
- ImGui integration
- Lua Scripting Support
- Lua level loading

Future features:

- Separation of engine and game code
- Editor
- Physics
- Sound
- More robust Lua scripting support

## How to build

``vcpkg install .``

``mkdir build && cd build``

``cmake ..``

``cmake --build .``

## How to run

``cd build``

``./Engine2D``

## Screenshots

![image](assets/repository/anim.gif)
![image](assets/repository/level2.png)
![image](assets/repository/level1.png)
![image](assets/repository/debug.png)
