# Linterra: A Voxel Engine in C++ & OpenGL

**Project Status:** In Development

This project is an ongoing exploration into building a voxel engine from scratch. It is not a playable game yet — the focus so far has been designing the underlying systems that make an infinite, block-based world possible.

> ⚠️ **Platform Notice:** Currently confirmed to build and run on macOS only, due to platform-specific OpenGL headers (`<OpenGL/gl.h>`). Adding Windows and Linux support is a key goal for the next phase.

---

## What is Linterra?

Linterra is a from-scratch, Minecraft-style voxel engine written in modern C++ and OpenGL. The goal isn't cloning gameplay — it's understanding and building the hard technical pieces: procedural generation, chunked world systems, GPU-efficient meshing, and real-time rendering architecture.

[Textures were received from Faithful 64x pack](https://faithfulpack.net/)

![linterra-demo](/docs/milestone2.gif)

## Implemented Features

### Milestone 1 — Engine Foundations

The initial milestone focused on building the foundation required for an infinite voxel world.

**Dynamic Chunk Management**

- A `ChunkManager` loads and unloads chunks based on camera position
- Chunks stored in an `std::unordered_map` keyed by a custom `ChunkPosition` hash
- Fixed render distance; out-of-range chunks are pruned each frame
- Supports a theoretically infinite world while keeping memory bounded

**First-Person Camera**

- Standard fly-through camera with yaw/pitch mouse-look
- WASD + Space/Shift movement
- Adjustable speed, sensitivity, and FOV

**Modern Shader Abstraction**

- A `Shader` class handles reading, compiling, linking shader programs
- Uniform location caching to reduce driver calls

**Basic Rendering Pipeline**

- Window and input via GLFW
- OpenGL loading via GLAD
- Core render loop with event dispatching and input callbacks

### Milestone 2 — World Rendering & Performance

This milestone added actual voxel content, terrain generation, rendering efficiency, and early performance passes.

**Voxel Meshing System**

- Per-chunk face culling: only visible faces are emitted
- Generates a vertex buffer for each chunk at creation time
- Significantly reduces geometry vs. naïve full-cube rendering

**GPU Geometry Upload**

- Each chunk owns a VAO and VBO for its mesh
- Static draw buffers; draw calls are per chunk
- Deterministic creation and teardown of GPU resources

**View-Frustum Culling**

- Each chunk performs frustum intersection tests against camera planes
- Out-of-view chunks are skipped entirely in the render loop
- Big performance gains as world scale increases

**Noise-Based Procedural Terrain**

- Heightmap generation using layered Perlin noise
- Produces hills, slopes, and believable terrain variation across infinite chunks

**Block Storage System**

- Chunks contain a fixed 3D block array with typed block IDs
- Enables meaningful terrain data, not placeholder geometry

**Camera Math Improvements**

- Corrected right/front/up vector derivation
- More stable and consistent movement/orientation behavior

**Shader & Error Handling Improvements**

- Better visibility for shader compilation errors
- Validation for shader program linking
- Basic logging hooks added in critical paths

---

## Technical Stack

- **Language:** C++ Z23
- **Graphics:** OpenGL 3.3+
- **Libraries:**
  - **GLFW** — windowing & input
  - **GLAD** — OpenGL function loading
  - **GLM** — mathematical foundations (matrices, vectors)

---

## Building & Running

### Prerequisites

- macOS 12+
- C++20-capable compiler (Clang recommended)
- CMake 3.15+
- Homebrew recommended:
  ```bash
  brew install glfw glm
  ```

### Build

```bash
git clone https://github.com/amodhakal/linterra.git
cd linterra
mkdir build && cd build
cmake ..
make
```

### Run

```bash
./linterra
```
