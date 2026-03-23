# TinyEngine

TinyEngine is a lightweight 2D game engine/framework learning project focused on proving core engine fundamentals: game loop, rendering pipeline groundwork, resource lifetime control, and ECS-oriented architecture thinking.

## Progress Log (Day1 - Day5)

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