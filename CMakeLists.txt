cmake_minimum_required(VERSION 3.21)
project(TinyEngine VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(TinyEngine
	main.cpp
	src/Core/GameLoop.cpp
	src/Core/Log.cpp
	src/Core/Timer.cpp
	src/Core/Window.cpp
	src/Graphics/Renderer2D.cpp
	src/Graphics/Shader.cpp
	src/Math/Matrix4x4.cpp
	src/Math/Vector2.cpp
	src/Math/Vector3.cpp
	src/Math/Vector4.cpp
)
target_include_directories(TinyEngine
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Dependencies.cmake)
target_link_libraries(TinyEngine PRIVATE SDL2::SDL2-static)
add_executable(TinyEngineMathTests
	tests/MathTests.cpp
	src/Math/Matrix4x4.cpp
	src/Math/Vector2.cpp
	src/Math/Vector3.cpp
	src/Math/Vector4.cpp
)
target_include_directories(TinyEngineMathTests
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)
enable_testing()
add_test(NAME TinyEngineMathTests COMMAND TinyEngineMathTests)
