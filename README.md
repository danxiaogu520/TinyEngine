# TinyEngine

TinyEngine is a lightweight 2D game engine/framework learning project.

## Day1 Progress

- CMake project skeleton initialized
- Third-party dependency management added via CMake FetchContent
- SDL2 backend selected

## Day2 Progress

- Core window wrapper implemented with SDL2
- Basic event dispatch implemented (Quit and WindowResized)
- Entry point switched to a minimal window/event loop demo

## Day3 Progress

- Added Timer with delta time and elapsed time
- Added GameLoop with init/event/update/shutdown callbacks
- Main entry now uses GameLoop callbacks

## Day4 Started (Collaborative)

- Added Vector2 API and source file skeleton
- Left core math logic as TODO(user): +, -, *, /, Dot, Cross, Normalize

## Configure

```bash
cmake -S . -B build -DTINYENGINE_FETCH_DEPS=ON
```

Use local-only mode (no dependency download):

```bash
cmake -S . -B build -DTINYENGINE_FETCH_DEPS=OFF
```

## Build

```bash
cmake --build build
```