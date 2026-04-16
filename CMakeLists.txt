cmake_minimum_required(VERSION 3.21)
# Keep legacy third-party CMake scripts (e.g. glad) configurable under CMake 4.x.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(TinyEngine VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(TinyEngine
	main.cpp
	src/Core/GameLoop.cpp
	src/Core/Input.cpp
	src/Core/Log.cpp
	src/Core/Timer.cpp
	src/Core/Window.cpp
	src/Graphics/IndexBuffer.cpp
	src/Graphics/OrthographicCamera.cpp
	src/Graphics/Renderer2D.cpp
	src/Graphics/Shader.cpp
	src/Graphics/Texture2D.cpp
	src/Graphics/VertexArray.cpp
	src/Graphics/VertexBuffer.cpp
	src/Math/Matrix4x4.cpp
	src/Math/Vector2.cpp
	src/Math/Vector3.cpp
	src/Math/Vector4.cpp
	src/Resource/AudioClip.cpp
)
target_include_directories(TinyEngine
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Dependencies.cmake)
target_include_directories(TinyEngine PRIVATE ${stb_SOURCE_DIR})
target_link_libraries(TinyEngine PRIVATE SDL2::SDL2-static)
target_link_libraries(TinyEngine PRIVATE OpenGL::GL glad)

add_executable(TinyEngineMathTests
	tests/MathTests.cpp
	src/Graphics/OrthographicCamera.cpp
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

add_executable(TinyEngineResourceTests
	tests/ResourceTests.cpp
	src/Resource/AudioClip.cpp
)
target_include_directories(TinyEngineResourceTests
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(TinyEngineResourceTests PRIVATE SDL2::SDL2-static)

add_executable(TinyEngineSceneTests
	tests/SceneTests.cpp
)
target_include_directories(TinyEngineSceneTests
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

enable_testing()
add_test(NAME TinyEngineMathTests COMMAND TinyEngineMathTests)
add_test(NAME TinyEngineECSTests COMMAND TinyEngineECSTests)
add_test(NAME TinyEngineResourceTests COMMAND TinyEngineResourceTests)
add_test(NAME TinyEngineSceneTests COMMAND TinyEngineSceneTests)
