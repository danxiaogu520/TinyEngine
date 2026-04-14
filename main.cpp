#include <cmath>
#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glad/glad.h>

#include "TinyEngine/Core/Event.h"
#include "TinyEngine/Core/GameLoop.h"
#include "TinyEngine/Core/Log.h"
#include "TinyEngine/Core/Window.h"
#include "TinyEngine/Graphics/Renderer2D.h"
#include "TinyEngine/Math/Vector2.h"

int main() {
	TinyEngine::Core::Window window({"胆小菇QAQ", 1280, 720});
	TinyEngine::Core::GameLoop loop;

	SDL_GLContext glContext = nullptr;
	bool glReady = false;
	TinyEngine::Graphics::Renderer2D renderer;
	float elapsedSeconds = 0.0f;

	TinyEngine::Core::GameLoopCallbacks callbacks;
	callbacks.onInit = [&]() {
		TinyEngine::Core::Log::Info("Game loop started.");
		SDL_Window* nativeWindow = window.GetNativeHandle();
		if (nativeWindow == nullptr) {
			TinyEngine::Core::Log::Error("Native SDL window is null.");
			window.RequestClose();
			return;
		}

		glContext = SDL_GL_CreateContext(nativeWindow);
		if (glContext == nullptr) {
			TinyEngine::Core::Log::Error("Failed to create OpenGL context.");
			window.RequestClose();
			return;
		}

		if (SDL_GL_MakeCurrent(nativeWindow, glContext) != 0) {
			TinyEngine::Core::Log::Error("Failed to make OpenGL context current.");
			SDL_GL_DeleteContext(glContext);
			glContext = nullptr;
			window.RequestClose();
			return;
		}

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress))) {
			TinyEngine::Core::Log::Error("Failed to initialize glad.");
			SDL_GL_DeleteContext(glContext);
			glContext = nullptr;
			window.RequestClose();
			return;
		}

		SDL_GL_SetSwapInterval(1);

		if (!renderer.Initialize(window)) {
			TinyEngine::Core::Log::Error("Renderer2D initialization failed.");
			window.RequestClose();
			return;
		}
		renderer.SetClearColor(0.08f, 0.10f, 0.16f, 1.0f);
		glReady = true;
	};
	callbacks.onEvent = [&](const TinyEngine::Core::Event& event) {
		if (event.type == TinyEngine::Core::EventType::WindowResized) {
			std::cout << "Window resized to: " << event.width << "x" << event.height << std::endl;
			if (glReady) {
				renderer.OnResize(event.width, event.height);
			}
		}
	};
	callbacks.onUpdate = [&](const double deltaTimeSeconds) {
		if (!glReady) {
			return;
		}

		elapsedSeconds += static_cast<float>(deltaTimeSeconds);
		const float offsetX = 220.0f + std::sin(elapsedSeconds) * 120.0f;

		renderer.BeginFrame();
		renderer.DrawQuad(TinyEngine::Math::Vector2(offsetX, 180.0f), TinyEngine::Math::Vector2(220.0f, 220.0f));
		renderer.EndFrame();
	};
	callbacks.onShutdown = [&]() {
		renderer.Shutdown();
		if (glContext != nullptr) {
			SDL_GL_DeleteContext(glContext);
			glContext = nullptr;
		}
		TinyEngine::Core::Log::Info("Game loop stopped.");
	};

	if (!loop.Run(window, callbacks)) {
		std::cerr << "Failed to start game loop." << std::endl;
		return 1;
	}

	return 0;
}
