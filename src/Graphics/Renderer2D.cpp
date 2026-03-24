#include "TinyEngine/Graphics/Renderer2D.h"

#include <algorithm>

#include <SDL.h>

#include "TinyEngine/Core/Window.h"

namespace TinyEngine::Graphics {
	namespace {
		std::uint8_t FloatToByte(float value) {
			const float clamped = std::clamp(value, 0.0f, 1.0f);
			return static_cast<std::uint8_t>(clamped * 255.0f);
		}
	} // namespace

	bool Renderer2D::Initialize(Core::Window& window) {
		if (m_renderer != nullptr) {
			return true;
		}

		SDL_Window* nativeWindow = window.GetNativeHandle();
		if (nativeWindow == nullptr) {
			return false;
		}

		m_renderer = SDL_CreateRenderer(nativeWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (m_renderer == nullptr) {
			m_renderer = SDL_CreateRenderer(nativeWindow, -1, SDL_RENDERER_SOFTWARE);
			if (m_renderer == nullptr) {
				return false;
			}
		}

		return true;
	}

	void Renderer2D::Shutdown() {
		if (m_renderer != nullptr) {
			SDL_DestroyRenderer(m_renderer);
			m_renderer = nullptr;
		}
	}

	void Renderer2D::SetClearColor(float r, float g, float b, float a) {
		m_clearR = FloatToByte(r);
		m_clearG = FloatToByte(g);
		m_clearB = FloatToByte(b);
		m_clearA = FloatToByte(a);
	}

	void Renderer2D::BeginFrame() {
		if (m_renderer == nullptr) {
			return;
		}

		SDL_SetRenderDrawColor(m_renderer, m_clearR, m_clearG, m_clearB, m_clearA);
		SDL_RenderClear(m_renderer);
	}

	void Renderer2D::EndFrame() {
		if (m_renderer == nullptr) {
			return;
		}

		SDL_RenderPresent(m_renderer);
	}

	void Renderer2D::DrawFilledRect(int x, int y, int width, int height, float r, float g, float b, float a) {
		if (m_renderer == nullptr) {
			return;
		}

		if (width <= 0 || height <= 0) {
			return;
		}

		SDL_SetRenderDrawColor(m_renderer, FloatToByte(r), FloatToByte(g), FloatToByte(b), FloatToByte(a));
		SDL_Rect rect{x, y, width, height};
		SDL_RenderFillRect(m_renderer, &rect);
	}

	void Renderer2D::OnResize(int width, int height) {
		if (m_renderer == nullptr) {
			return;
		}

		SDL_Rect viewport{0, 0, width, height};
		SDL_RenderSetViewport(m_renderer, &viewport);
	}
} // namespace TinyEngine::Graphics
