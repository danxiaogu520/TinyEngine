#include "TinyEngine/Core/Window.h"

#include <cstdint>
#include <SDL.h>

namespace TinyEngine::Core {
	Window::Window(WindowSpec spec) : m_spec(std::move(spec)) {}

	Window::~Window() {
		Shutdown();
	}

	bool Window::Initialize() {
		if (m_nativeWindow != nullptr) {
			return true;
		}

		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			return false;
		}

		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3) != 0 ||
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3) != 0 ||
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0) {
			SDL_Quit();
			return false;
		}

		const std::uint32_t windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

		m_nativeWindow = SDL_CreateWindow(
			m_spec.title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_spec.width,
			m_spec.height,
			windowFlags
		);

		if (m_nativeWindow == nullptr) {
			SDL_Quit();
			return false;
		}

		return true;
	}

	void Window::Shutdown() {
		if (m_nativeWindow != nullptr) {
			SDL_DestroyWindow(m_nativeWindow);
			m_nativeWindow = nullptr;
		}

		SDL_Quit();
	}

	bool Window::PollEvent(Event& outEvent) {
		outEvent = {};

		SDL_Event sdlEvent{};
		if (SDL_PollEvent(&sdlEvent) == 0) {
			return false;
		}

		if (sdlEvent.type == SDL_QUIT) {
			outEvent.type = EventType::Quit;
			m_shouldClose = true;
			return true;
		}

		if (sdlEvent.type == SDL_WINDOWEVENT && sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
			outEvent.type = EventType::WindowResized;
			outEvent.width = sdlEvent.window.data1;
			outEvent.height = sdlEvent.window.data2;
			return true;
		}

		if (sdlEvent.type == SDL_KEYDOWN) {
			outEvent.type = EventType::KeyDown;
			outEvent.keyCode = sdlEvent.key.keysym.sym;
			outEvent.isRepeat = sdlEvent.key.repeat != 0;
			return true;
		}

		if (sdlEvent.type == SDL_KEYUP) {
			outEvent.type = EventType::KeyUp;
			outEvent.keyCode = sdlEvent.key.keysym.sym;
			outEvent.isRepeat = false;
			return true;
		}

		if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
			outEvent.type = EventType::MouseButtonDown;
			outEvent.mouseButton = sdlEvent.button.button;
			outEvent.mouseX = sdlEvent.button.x;
			outEvent.mouseY = sdlEvent.button.y;
			return true;
		}

		if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
			outEvent.type = EventType::MouseButtonUp;
			outEvent.mouseButton = sdlEvent.button.button;
			outEvent.mouseX = sdlEvent.button.x;
			outEvent.mouseY = sdlEvent.button.y;
			return true;
		}

		if (sdlEvent.type == SDL_MOUSEMOTION) {
			outEvent.type = EventType::MouseMoved;
			outEvent.mouseX = sdlEvent.motion.x;
			outEvent.mouseY = sdlEvent.motion.y;
			outEvent.mouseDeltaX = sdlEvent.motion.xrel;
			outEvent.mouseDeltaY = sdlEvent.motion.yrel;
			return true;
		}

		if (sdlEvent.type == SDL_MOUSEWHEEL) {
			outEvent.type = EventType::MouseWheel;
			outEvent.wheelX = sdlEvent.wheel.x;
			outEvent.wheelY = sdlEvent.wheel.y;
			return true;
		}

		outEvent.type = EventType::None;
		return true;
	}

	SDL_Window* Window::GetNativeHandle() const {
		return m_nativeWindow;
	}

	bool Window::ShouldClose() const {
		return m_shouldClose;
	}

	void Window::RequestClose() {
		m_shouldClose = true;
	}
} // namespace TinyEngine::Core
