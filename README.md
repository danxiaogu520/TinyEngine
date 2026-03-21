# TinyEngine

TinyEngine is a lightweight 2D game engine/framework learning project.

## Day1 Progress

- CMake project skeleton initialized
- Third-party dependency management added via CMake FetchContent
- Backend switch options prepared for SDL2 or GLFW

## Configure

```bash
cmake -S . -B build -DTINYENGINE_FETCH_DEPS=ON -DTINYENGINE_USE_SDL2=ON -DTINYENGINE_USE_GLFW=OFF
```

Use local-only mode (no dependency download):

```bash
cmake -S . -B build -DTINYENGINE_FETCH_DEPS=OFF
```

## Build

```bash
cmake --build build
```