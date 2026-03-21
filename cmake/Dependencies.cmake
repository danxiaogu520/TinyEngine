include(FetchContent)

if(TINYENGINE_USE_SDL2 AND TINYENGINE_USE_GLFW)
    message(FATAL_ERROR "Select only one backend: SDL2 or GLFW.")
endif()

if(TINYENGINE_USE_SDL2)
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.10
    )
    FetchContent_MakeAvailable(SDL2)
endif()

if(TINYENGINE_USE_GLFW)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(glfw)
endif()
