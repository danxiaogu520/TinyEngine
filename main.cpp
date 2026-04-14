#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glad/glad.h>
#include <format>

#include "TinyEngine/Core/Event.h"
#include "TinyEngine/Core/GameLoop.h"
#include "TinyEngine/Core/Log.h"
#include "TinyEngine/Core/Window.h"
#include "TinyEngine/Graphics/Renderer2D.h"

int main() {
	TinyEngine::Core::Window window({"胆小菇QAQ", 1280, 720});
	TinyEngine::Core::GameLoop loop;

	SDL_GLContext glContext = nullptr;
	bool glReady = false;
	TinyEngine::Graphics::Renderer2D renderer;
	int width = 1280;
	int height = 720;

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
			if (width != event.width || height != event.height) {
				TinyEngine::Core::Log::Info(std::format("Window resized to: {} x {}.", event.width, event.height));
			}
			width = event.width;
			height = event.height;

			if (glReady) {
				renderer.OnResize(event.width, event.height);
			}
		}
	};
	callbacks.onUpdate = [&](const double deltaTimeSeconds) {
		if (!glReady) {
			return;
		}
		renderer.BeginFrame();
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
		TinyEngine::Core::Log::Error("Failed to start game loop.");
		return 1;
	}

	return 0;
}
