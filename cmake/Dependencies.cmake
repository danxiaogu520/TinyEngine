include(FetchContent)

set(SDL_SHARED OFF CACHE BOOL "Build shared SDL library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build static SDL library" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Build SDL test library" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Build SDL tests" FORCE)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.10
)

FetchContent_MakeAvailable(SDL2)

if (TINYENGINE_ENABLE_OPENGL)
    find_package(OpenGL REQUIRED)

    FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG v0.1.36
    )

    FetchContent_MakeAvailable(glad)
endif()
