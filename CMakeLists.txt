cmake_minimum_required(VERSION 3.21)
project(TinyEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(TINYENGINE_FETCH_DEPS "Download third-party dependencies via FetchContent" ON)

add_executable(TinyEngine
	main.cpp
	src/Core/GameLoop.cpp
	src/Core/Timer.cpp
	src/Core/Window.cpp
	src/Math/Vector2.cpp
)

target_include_directories(TinyEngine
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(TINYENGINE_FETCH_DEPS)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Dependencies.cmake)
else()
	find_package(SDL2 CONFIG REQUIRED)
endif()

if(TARGET SDL2::SDL2-static)
	target_link_libraries(TinyEngine PRIVATE SDL2::SDL2-static)
elseif(TARGET SDL2::SDL2)
	target_link_libraries(TinyEngine PRIVATE SDL2::SDL2)

	if(WIN32)
		add_custom_command(TARGET TinyEngine POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_RUNTIME_DLLS:TinyEngine>
				$<TARGET_FILE_DIR:TinyEngine>
			COMMAND_EXPAND_LISTS
		)
	endif()
else()
	message(FATAL_ERROR "SDL2 target not found. Expected SDL2::SDL2-static or SDL2::SDL2")
endif()
