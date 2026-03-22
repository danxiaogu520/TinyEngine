#pragma once

#include <string>

#include "TinyEngine/Core/Event.h"

struct SDL_Window;

namespace TinyEngine::Core {
	struct WindowSpec {
		std::string title = "TinyEngine";
		int width = 1280;
		int height = 720;
	};

	class Window {
	public:
		explicit Window(WindowSpec spec = {});
		~Window();

		bool Initialize();
		void Shutdown();

		bool PollEvent(Event& outEvent);

		[[nodiscard]] bool ShouldClose() const;
		void RequestClose();

	private:
		WindowSpec m_spec;
		SDL_Window* m_nativeWindow = nullptr;
		bool m_shouldClose = false;
	};
} // namespace TinyEngine::Core
