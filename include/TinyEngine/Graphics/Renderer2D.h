#pragma once

#include <cstdint>
#include <memory>

#include "TinyEngine/Math/Matrix4x4.h"

namespace TinyEngine::Math {
	struct Vector2;
}

namespace TinyEngine::Core {
	class Window;
}

namespace TinyEngine::Graphics {
	class IndexBuffer;
	class Shader;
	class Texture2D;
	class VertexArray;
	class VertexBuffer;
}

namespace TinyEngine::Graphics {
	class Renderer2D {
	public:
		Renderer2D();
		~Renderer2D();

		Renderer2D(const Renderer2D&) = delete;
		Renderer2D& operator=(const Renderer2D&) = delete;
		Renderer2D(Renderer2D&&) = delete;
		Renderer2D& operator=(Renderer2D&&) = delete;

		bool Initialize(Core::Window& window);
		void Shutdown();

		void SetClearColor(float r, float g, float b, float a = 1.0f);
		void BeginFrame();
		void EndFrame();
		void DrawQuad(const Math::Vector2& position, const Math::Vector2& size, const Texture2D* texture = nullptr);

		void OnResize(int width, int height);

	private:
		bool BuildQuadPipeline();
		void RebuildProjection(int width, int height);

		Core::Window* m_window = nullptr;
		std::unique_ptr<Shader> m_shader;
		std::unique_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<VertexBuffer> m_vertexBuffer;
		std::shared_ptr<IndexBuffer> m_indexBuffer;
		Math::Matrix4x4 m_projection = Math::Matrix4x4::Identity();
		std::uint32_t m_whiteTextureId = 0;
		float m_clearR = 0.10f;
		float m_clearG = 0.12f;
		float m_clearB = 0.17f;
		float m_clearA = 1.0f;
	};
} // namespace TinyEngine::Graphics
