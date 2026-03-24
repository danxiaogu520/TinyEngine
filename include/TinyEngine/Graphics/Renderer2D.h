#pragma once

#include <cstdint>

namespace TinyEngine::Core {
	class Window;
}

struct SDL_Renderer;

namespace TinyEngine::Graphics {
	class Renderer2D {
	public:
		bool Initialize(Core::Window& window);
		void Shutdown();

		void SetClearColor(float r, float g, float b, float a = 1.0f);
		void BeginFrame();
		void EndFrame();
		void DrawFilledRect(int x, int y, int width, int height, float r, float g, float b, float a = 1.0f);

		void OnResize(int width, int height);

	private:
		SDL_Renderer* m_renderer = nullptr;
		std::uint8_t m_clearR = 26;
		std::uint8_t m_clearG = 30;
		std::uint8_t m_clearB = 44;
		std::uint8_t m_clearA = 255;
	};
} // namespace TinyEngine::Graphics
