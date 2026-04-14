cmake_minimum_required(VERSION 3.21)
# Keep legacy third-party CMake scripts (e.g. glad) configurable under CMake 4.x.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(TinyEngine VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(TINYENGINE_ENABLE_OPENGL "Enable OpenGL backend for shader and GPU buffer pipeline" ON)

add_executable(TinyEngine
	main.cpp
	src/Core/GameLoop.cpp
	src/Core/Log.cpp
	src/Core/Timer.cpp
	src/Core/Window.cpp
	src/Graphics/IndexBuffer.cpp
	src/Graphics/Renderer2D.cpp
	src/Graphics/Shader.cpp
	src/Graphics/VertexArray.cpp
	src/Graphics/VertexBuffer.cpp
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

if (TINYENGINE_ENABLE_OPENGL)
	target_compile_definitions(TinyEngine PRIVATE TINYENGINE_ENABLE_OPENGL)
	target_link_libraries(TinyEngine PRIVATE OpenGL::GL glad)
endif()

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

add_executable(TinyEngineECSTests
	tests/ECSTests.cpp
)
target_include_directories(TinyEngineECSTests
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_test(NAME TinyEngineMathTests COMMAND TinyEngineMathTests)
add_test(NAME TinyEngineECSTests COMMAND TinyEngineECSTests)
