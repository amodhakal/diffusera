# Linterra: A Voxel Engine in C++ & OpenGL

**Project Status: In Development.** The features described below represent the completion of the first major milestone: building a foundational engine. The project is not yet a playable game but serves as a demonstration of the core architecture.

> ⚠️ **Platform Notice:** Currently, this project is only confirmed to build and run on **macOS**. This is due to platform-specific header includes (`<OpenGL/gl.h>`) and the initial development environment. Adding support for Windows and Linux is a high-priority goal for the next development phase.



## What is Linterra?

Linterra is a personal project to build a Minecraft-style voxel engine from the ground up using modern C++ and OpenGL. The primary goal is to explore the complex challenges of procedural world generation, rendering performance with large datasets, and engine architecture.


![linterra-demo](/docs/milestone1.gif)

---
## Core Features Implemented (Milestone 1: The Foundation)

This first milestone was successfully completed and focused on creating a robust and extensible foundation for the engine.

- **Dynamic Chunk Management:** The world is managed by a `ChunkManager` that dynamically loads and unloads chunks based on the camera's position. This ensures a theoretically infinite world without consuming infinite memory.
  - Chunks are stored in an `std::unordered_map` using a custom hash for a `ChunkPosition` struct, allowing for O(1) average time complexity for chunk lookups.
  - A render distance is defined, and chunks outside this radius are automatically pruned from memory each frame.
- **First-Person Camera Controller:** A standard fly-through camera with mouse-look (pitch/yaw) and WASD + Space/Shift movement controls. Camera properties like speed, sensitivity, and FOV are easily configurable.
- **Abstracted OpenGL Shader Class:** A `Shader` class that handles the reading, compiling, and linking of GLSL vertex and fragment shaders from file. It also includes a simple system for caching uniform locations to improve performance.
- **Basic Rendering Pipeline:** A core application loop built with GLFW for window and input management, and GLAD for OpenGL function loading. The engine initializes a window, manages a render loop, and handles user input via callbacks.

---

## Technical Stack

- **Language:** C++20
- **Graphics API:** OpenGL 3.3+
- **Libraries:**
  - **GLFW:** Windowing and input
  - **GLAD:** OpenGL extension loading
  - **GLM:** OpenGL mathematics (vectors, matrices)

---

## How to Build and Run

1.  **Prerequisites:**

    - **macOS** 26.1
    - A C++ compiler supporting C++20 (e.g., Clang)
    - CMake 3.15+
    - Homebrew for dependency management is recommended:
      ```bash
      brew install glfw glm
      ```

2.  **Build Steps:**

    ```bash
    git clone https://github.com/amodhakal/linterra.git
    cd linterra
    mkdir build && cd build
    cmake ..
    make
    ```

3.  **Running:**
    ```bash
    ./linterra
    ```
