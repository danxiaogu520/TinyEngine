cmake_minimum_required(VERSION 3.21)
project(TinyEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(TINYENGINE_FETCH_DEPS "Download third-party dependencies via FetchContent" ON)
option(TINYENGINE_USE_SDL2 "Use SDL2 as the windowing/rendering backend" ON)
option(TINYENGINE_USE_GLFW "Use GLFW as the windowing backend" OFF)

add_executable(TinyEngine
	main.cpp
)

target_include_directories(TinyEngine
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(TINYENGINE_FETCH_DEPS)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Dependencies.cmake)

	if(TARGET SDL2::SDL2)
		target_link_libraries(TinyEngine PRIVATE SDL2::SDL2)
	endif()

	if(TARGET glfw)
		target_link_libraries(TinyEngine PRIVATE glfw)
	endif()
endif()
