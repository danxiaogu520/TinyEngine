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

## Development Build

### Configure (Fetch dependencies)

```bash
cmake -S . -B build -DTINYENGINE_FETCH_DEPS=ON
```

### Configure (Use local installed SDL2)

```bash
cmake -S . -B build -DTINYENGINE_FETCH_DEPS=OFF
```

### Build

```bash
cmake --build build
```

## 30-Day Roadmap (Merged from plan.md)

### Stage 1: Foundation and math system (Day1 - Day7)
1. Day1: Configure project environment. Finalize CMakeLists and third-party dependency management with SDL2.
2. Day2: Core layer. Implement Window abstraction to wrap low-level window creation and event dispatch.
3. Day3: Core layer. Implement GameLoop and Timer for delta-time based updates.
4. Day4: Math layer. Implement Vector2 operations: arithmetic, dot, cross, normalization.
5. Day5: Math layer. Implement Vector3 and Vector4 plus utility operations.
6. Day6: Math layer. Implement Matrix4x4, including translation, scaling, and orthographic projection.
7. Day7: Review and test day. Add lightweight Log module and math unit tests.

### Stage 2: Rendering pipeline encapsulation (Day8 - Day14)
1. Day8: Graphics layer. Initialize rendering context and implement clear/present flow.
2. Day9: Graphics layer. Implement Shader loading, compile, and link lifecycle.
3. Day10: Graphics layer. Implement VertexArray, VertexBuffer, and IndexBuffer wrappers.
4. Day11: Graphics layer. Implement primitive drawing support (Draw Quad).
5. Day12: Graphics layer. Integrate stb_image and implement Texture2D loading and GPU upload wrapper.
6. Day13: Graphics layer. Build OrthographicCamera with matrix support.
7. Day14: Refactor to Renderer2D API such as DrawQuad(position, size, texture).

### Stage 3: Resource and ECS (Day15 - Day22)
1. Day15: Resource layer. Define ResourceManager interface and resource handle ownership strategy.
2. Day16: Resource layer. Implement texture cache to avoid duplicate loading.
3. Day17: Resource layer. Integrate audio loading and cache strategy.
4. Day18: ECS layer. Define entity identifiers and component model.
5. Day19: ECS layer. Implement registry and component storage strategy.
6. Day20: ECS layer. Add core components: TransformComponent and SpriteRendererComponent.
7. Day21: ECS layer. Implement RenderSystem to iterate visible sprite entities and submit draw calls.
8. Day22: Integration review for ECS + Renderer + Resource Manager.

### Stage 4: Gameplay and demo (Day23 - Day30)
1. Day23: Input layer. Wrap keyboard and mouse state querying and event hooks.
2. Day24: ECS extension. Add gameplay system such as PlayerControllerSystem.
3. Day25: Scene system. Add Scene abstraction and lifecycle around ECS registry.
4. Day26: Build a playable demo scene (for example Flappy Bird style or brick breaker style).
5. Day27: Finish demo interaction and AABB collision handling.
6. Day28: Add background music and effects using audio module.
7. Day29: Profile and optimize memory and runtime hotspots, optionally attempt batch rendering.
8. Day30: Final documentation and interview notes for architecture, trade-offs, and technical highlights.