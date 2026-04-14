#include "TinyEngine/Graphics/Renderer2D.h"

#include <algorithm>
#include <memory>

#include <SDL.h>
#include <glad/glad.h>

#include "TinyEngine/Core/Window.h"
#include "TinyEngine/Graphics/IndexBuffer.h"
#include "TinyEngine/Graphics/Shader.h"
#include "TinyEngine/Graphics/Texture2D.h"
#include "TinyEngine/Graphics/VertexArray.h"
#include "TinyEngine/Graphics/VertexBuffer.h"
#include "TinyEngine/Math/Vector2.h"

namespace TinyEngine::Graphics {
	namespace {
		float ClampUnit(const float value) {
			return std::clamp(value, 0.0f, 1.0f);
		}
	} // namespace

	Renderer2D::Renderer2D() = default;

	Renderer2D::~Renderer2D() {
		Shutdown();
	}

	bool Renderer2D::Initialize(Core::Window& window) {
		if (m_shader != nullptr && m_vertexArray != nullptr) {
			return true;
		}

		SDL_Window* nativeWindow = window.GetNativeHandle();
		if (nativeWindow == nullptr) {
			return false;
		}
		m_window = &window;

		if (!BuildQuadPipeline()) {
			Shutdown();
			return false;
		}

		unsigned int whitePixel = 0xFFFFFFFFu;
		glGenTextures(1, &m_whiteTextureId);
		if (m_whiteTextureId == 0) {
			Shutdown();
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, m_whiteTextureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whitePixel);
		glBindTexture(GL_TEXTURE_2D, 0);

		int width = 0;
		int height = 0;
		SDL_GetWindowSize(nativeWindow, &width, &height);
		OnResize(width, height);

		return true;
	}

	void Renderer2D::Shutdown() {
		if (m_whiteTextureId != 0) {
			glDeleteTextures(1, &m_whiteTextureId);
			m_whiteTextureId = 0;
		}

		if (m_vertexArray != nullptr) {
			m_vertexArray->Reset();
		}

		if (m_shader != nullptr) {
			m_shader->Reset();
		}

		m_vertexArray.reset();
		m_indexBuffer.reset();
		m_vertexBuffer.reset();
		m_shader.reset();
		m_window = nullptr;
	}

	void Renderer2D::SetClearColor(float r, float g, float b, float a) {
		m_clearR = ClampUnit(r);
		m_clearG = ClampUnit(g);
		m_clearB = ClampUnit(b);
		m_clearA = ClampUnit(a);
	}

	void Renderer2D::BeginFrame() {
		if (m_shader == nullptr || m_vertexArray == nullptr) {
			return;
		}

		glClearColor(m_clearR, m_clearG, m_clearB, m_clearA);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer2D::EndFrame() {
		if (m_window == nullptr) {
			return;
		}

		SDL_GL_SwapWindow(m_window->GetNativeHandle());
	}

	void Renderer2D::DrawQuad(const Math::Vector2& position, const Math::Vector2& size, const Texture2D* texture) {
		if (m_shader == nullptr || m_vertexArray == nullptr || size.x <= 0.0f || size.y <= 0.0f) {
			return;
		}

		const Math::Matrix4x4 model =
			Math::Matrix4x4::Translation(position.x, position.y, 0.0f) *
			Math::Matrix4x4::Scale(size.x, size.y, 1.0f);
		const Math::Matrix4x4 mvp = m_projection * model;

		const std::uint32_t programId = m_shader->GetProgramId();
		const GLint uMvpLocation = glGetUniformLocation(programId, "u_MVP");
		const GLint uTextureLocation = glGetUniformLocation(programId, "u_Texture");

		m_shader->Bind();
		glUniformMatrix4fv(uMvpLocation, 1, GL_TRUE, mvp.m);
		glUniform1i(uTextureLocation, 0);

		if (texture != nullptr && texture->IsUploaded() && texture->GetTextureId() != 0) {
			texture->Bind(0);
		} else {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_whiteTextureId);
		}

		m_vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_vertexArray->IndexCount()), GL_UNSIGNED_INT, nullptr);
		m_vertexArray->Unbind();
		m_shader->Unbind();
	}

	void Renderer2D::OnResize(const int width, const int height) {
		if (width <= 0 || height <= 0) {
			return;
		}

		glViewport(0, 0, width, height);
		RebuildProjection(width, height);
	}

	bool Renderer2D::BuildQuadPipeline() {
		m_shader = std::make_unique<Shader>();
		m_vertexArray = std::make_unique<VertexArray>();
		m_vertexBuffer = std::make_shared<VertexBuffer>();
		m_indexBuffer = std::make_shared<IndexBuffer>();

		constexpr float quadVertices[] = {
			0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
		};
		constexpr std::uint32_t quadIndices[] = {0, 1, 2, 2, 3, 0};

		constexpr const char* vertexSource =
			"#version 330 core\n"
			"layout(location = 0) in vec4 a_PositionTex;\n"
			"uniform mat4 u_MVP;\n"
			"out vec2 v_TexCoord;\n"
			"void main() {\n"
			"  v_TexCoord = a_PositionTex.zw;\n"
			"  gl_Position = u_MVP * vec4(a_PositionTex.xy, 0.0, 1.0);\n"
			"}\n";

		constexpr const char* fragmentSource =
			"#version 330 core\n"
			"in vec2 v_TexCoord;\n"
			"uniform sampler2D u_Texture;\n"
			"out vec4 FragColor;\n"
			"void main() {\n"
			"  FragColor = texture(u_Texture, v_TexCoord);\n"
			"}\n";

		if (!m_shader->LoadFromSource(vertexSource, fragmentSource)) {
			return false;
		}

		if (!m_vertexBuffer->SetData(quadVertices, sizeof(quadVertices) / sizeof(float), 4 * sizeof(float))) {
			return false;
		}

		if (!m_indexBuffer->SetData(quadIndices, sizeof(quadIndices) / sizeof(std::uint32_t))) {
			return false;
		}

		m_vertexArray->SetVertexBuffer(m_vertexBuffer);
		m_vertexArray->SetIndexBuffer(m_indexBuffer);
		return m_vertexArray->Build();
	}

	void Renderer2D::RebuildProjection(const int width, const int height) {
		m_projection = Math::Matrix4x4::Orthographic(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
	}
} // namespace TinyEngine::Graphics
