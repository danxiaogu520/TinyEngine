#include <iostream>
#include <memory>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <glad/glad.h>

#include "TinyEngine/Core/Event.h"
#include "TinyEngine/Core/GameLoop.h"
#include "TinyEngine/Core/Log.h"
#include "TinyEngine/Core/Window.h"
#include "TinyEngine/Graphics/IndexBuffer.h"
#include "TinyEngine/Graphics/Shader.h"
#include "TinyEngine/Graphics/VertexArray.h"
#include "TinyEngine/Graphics/VertexBuffer.h"

int main() {
	TinyEngine::Core::Window window({"胆小菇QAQ", 1280, 720});
	TinyEngine::Core::GameLoop loop;

	SDL_GLContext glContext = nullptr;
	bool glReady = false;
	TinyEngine::Graphics::Shader shader;
	auto vertexBuffer = std::make_shared<TinyEngine::Graphics::VertexBuffer>();
	auto indexBuffer = std::make_shared<TinyEngine::Graphics::IndexBuffer>();
	TinyEngine::Graphics::VertexArray vertexArray;

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

		constexpr float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f,
		};
		constexpr std::uint32_t indices[] = {0, 1, 2};

		constexpr const char* vertexSource =
			"#version 330 core\n"
			"layout(location = 0) in vec3 aPos;\n"
			"void main() {\n"
			"  gl_Position = vec4(aPos, 1.0);\n"
			"}\n";

		constexpr const char* fragmentSource =
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main() {\n"
			"  FragColor = vec4(0.20, 0.82, 0.58, 1.0);\n"
			"}\n";

		if (!shader.LoadFromSource(vertexSource, fragmentSource)) {
			TinyEngine::Core::Log::Error("Shader compile/link failed.");
			window.RequestClose();
			return;
		}

		if (!vertexBuffer->SetData(vertices, sizeof(vertices) / sizeof(float), 3 * sizeof(float))) {
			TinyEngine::Core::Log::Error("VertexBuffer SetData failed.");
			window.RequestClose();
			return;
		}

		if (!indexBuffer->SetData(indices, sizeof(indices) / sizeof(std::uint32_t))) {
			TinyEngine::Core::Log::Error("IndexBuffer SetData failed.");
			window.RequestClose();
			return;
		}

		vertexArray.SetVertexBuffer(vertexBuffer);
		vertexArray.SetIndexBuffer(indexBuffer);

		if (!vertexArray.Build()) {
			TinyEngine::Core::Log::Error("VertexArray Build failed.");
			window.RequestClose();
			return;
		}

		glViewport(0, 0, 1280, 720);
		glClearColor(0.08f, 0.10f, 0.16f, 1.0f);
		glReady = true;
	};
	callbacks.onEvent = [&](const TinyEngine::Core::Event& event) {
		if (event.type == TinyEngine::Core::EventType::WindowResized) {
			std::cout << "Window resized to: " << event.width << "x" << event.height << std::endl;
			if (glReady) {
				glViewport(0, 0, event.width, event.height);
			}
		}
	};
	callbacks.onUpdate = [&](const double deltaTimeSeconds) {
		(void)deltaTimeSeconds;
		if (!glReady) {
			return;
		}

		glClear(GL_COLOR_BUFFER_BIT);
		shader.Bind();
		vertexArray.Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray.IndexCount()), GL_UNSIGNED_INT, nullptr);
		vertexArray.Unbind();
		shader.Unbind();
		SDL_GL_SwapWindow(window.GetNativeHandle());
	};
	callbacks.onShutdown = [&]() {
		vertexArray.Reset();
		shader.Reset();
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
