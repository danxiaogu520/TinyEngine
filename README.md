# TinyEngine

TinyEngine is a lightweight 2D game engine/framework learning project focused on proving core engine fundamentals: game loop, rendering pipeline groundwork, resource lifetime control, and ECS-oriented architecture thinking.

## Progress Log (Day1 - Day16)

### Day1 - Project bootstrap and dependency strategy
1. Built a clean CMake project entry with C++23 constraints.
2. Set up SDL2 dependency flow via FetchContent.
3. Added local dependency mode using find_package for machines that already have SDL2 installed.
4. Organized include and source directory structure for Core, Math, ECS, Graphics, and Resource modules.

### Day2 - Core window layer and event dispatch
1. Added window abstraction in Core module.
2. Wrapped SDL2 initialization and window lifecycle management.
3. Mapped native SDL events to engine events.
4. Implemented minimal event dispatch for Quit and WindowResized.

### Day3 - Game loop and timing system
1. Added Timer module to provide deltaTime and elapsed time.
2. Added GameLoop module with callback hooks: onInit, onEvent, onUpdate, onShutdown.
3. Switched app entry to callback-driven loop.
4. Introduced fixed timestep simulation pattern in update flow for stable behavior.

### Day4 - Vector2 math module
1. Added Vector2 data structure and arithmetic operators.
2. Implemented length, normalization, dot, and 2D cross scalar.
3. Added divide-by-zero guards and zero-length normalization protection.
4. Integrated Vector2 source into build target.

### Day5 - Vector3 and Vector4 math module
1. Added Vector3 and Vector4 APIs and source files.
2. Implemented arithmetic, dot, normalize for both.
3. Implemented 3D cross product for Vector3.
4. Corrected Vector3 mathematical edge cases and formula details.
5. Completed Vector4 implementation to remove duplicated manual placeholder work.

### Day6 - Matrix4x4 module
1. Added Matrix4x4 API and source files.
2. Implemented translation matrix.
3. Implemented scale matrix.
4. Implemented orthographic projection matrix.
5. Implemented matrix-matrix and matrix-vector multiplication.

### Day7 - Logging and math tests
1. Added lightweight Log module with Info, Warning, and Error levels.
2. Added math test executable for Vector2, Vector3, Vector4, and Matrix4x4.
3. Enabled CTest registration for TinyEngineMathTests.

### Day8 - Rendering layer bootstrap
1. Added Renderer2D wrapper based on SDL_Renderer.
2. Implemented renderer lifecycle: initialize, begin frame clear, end frame present, shutdown.
3. Added basic filled-rectangle draw API.
4. Connected rendering flow into game loop update path.
5. Hooked window resize events to renderer viewport update.
6. Added a visible moving box render demo.

### Day9 - Shader pipeline foundation
1. Added Shader class abstraction with lifecycle management.
2. Implemented vertex and fragment shader compile flow.
3. Implemented program link flow with detailed compile/link error reporting.
4. Unified runtime to OpenGL-first rendering pipeline.

### Day10 - GPU buffer and smoke test integration
1. Added GPU-capable VertexBuffer and IndexBuffer upload/bind lifecycle.
2. Added VertexArray build path that wires vertex attributes and index binding.
3. Integrated OpenGL smoke test path in app startup.
4. Removed dual-backend branch logic to keep one rendering path.

### Day11 - ECS minimal core loop
1. Added Entity ID model and Registry lifecycle (create/destroy/isAlive).
2. Added generic component storage with add/remove/has/get operations.
3. Added View query for multi-component entity iteration.
4. Added base system interface and scheduler update loop.
5. Added ECS tests covering registry behavior and a movement system update.

### Day12 - Texture2D and image decoding
1. Introduced stb_image dependency for image file decoding.
2. Added Texture2D wrapper with image loading, metadata, and error reporting.
3. Added OpenGL texture upload, bind/unbind, and resource lifecycle management.
4. Aligned texture path with always-on OpenGL runtime.

### Day13 - Orthographic camera foundation
1. Added OrthographicCamera abstraction with projection/view/view-projection matrices.
2. Added camera position and Z-rotation controls with automatic matrix recomputation.
3. Added Day13 camera tests into existing math test executable.

### Day14 - Renderer2D API refactor
1. Refactored Renderer2D to an OpenGL-backed pipeline and removed SDL_Renderer-specific rendering path.
2. Added a simple textured quad API: DrawQuad(position, size, texture).
3. Added automatic viewport/projection updates on window resize for screen-space rendering.
4. Switched app entry rendering flow to Renderer2D and validated a moving quad demo path.

### Day15 - Resource manager foundation
1. Added generic ResourceHandle based on weak references to support lightweight resource references.
2. Added ResourceManager base API for typed register/get/resolve/unload operations.
3. Added ResourceManager tests and integrated TinyEngineResourceTests into CTest.

### Day16 - Texture cache pool
1. Added load-or-get capability in ResourceManager to support cache-first loading behavior.
2. Added TextureCache abstraction for path-based texture caching with duplicate-instantiation prevention.
3. Extended resource tests to validate cache behavior and ensure repeated loads for the same key reuse a single instance.