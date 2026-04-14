#include "TinyEngine/Graphics/Texture2D.h"

#include <utility>

#if defined(TINYENGINE_ENABLE_OPENGL)
#include <glad/glad.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace TinyEngine::Graphics {
	Texture2D::~Texture2D() {
		Reset();
	}

	Texture2D::Texture2D(Texture2D&& other) noexcept : m_pixels(std::move(other.m_pixels)),
												  m_width(other.m_width),
												  m_height(other.m_height),
												  m_channelCount(other.m_channelCount),
												  m_textureId(other.m_textureId),
												  m_uploaded(other.m_uploaded),
												  m_lastError(std::move(other.m_lastError)) {
		other.m_width = 0;
		other.m_height = 0;
		other.m_channelCount = 0;
		other.m_textureId = 0;
		other.m_uploaded = false;
	}

	Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
		if (this == &other) {
			return *this;
		}

		Reset();

		m_pixels = std::move(other.m_pixels);
		m_width = other.m_width;
		m_height = other.m_height;
		m_channelCount = other.m_channelCount;
		m_textureId = other.m_textureId;
		m_uploaded = other.m_uploaded;
		m_lastError = std::move(other.m_lastError);

		other.m_width = 0;
		other.m_height = 0;
		other.m_channelCount = 0;
		other.m_textureId = 0;
		other.m_uploaded = false;
		return *this;
	}

	bool Texture2D::LoadFromFile(const std::string_view filePath, const bool flipVertically) {
		Reset();
		m_lastError.clear();

		stbi_set_flip_vertically_on_load(flipVertically ? 1 : 0);

		int width = 0;
		int height = 0;
		int channelCountInFile = 0;
		unsigned char* rawPixels = stbi_load(
			std::string(filePath).c_str(),
			&width,
			&height,
			&channelCountInFile,
			STBI_rgb_alpha
		);

		if (rawPixels == nullptr || width <= 0 || height <= 0) {
			m_lastError = "Failed to load image from file.";
			if (rawPixels != nullptr) {
				stbi_image_free(rawPixels);
			}
			return false;
		}

		m_width = width;
		m_height = height;
		m_channelCount = 4;

		const std::size_t byteCount = static_cast<std::size_t>(m_width) * static_cast<std::size_t>(m_height) * static_cast<std::size_t>(m_channelCount);
		m_pixels.assign(rawPixels, rawPixels + byteCount);
		stbi_image_free(rawPixels);

		m_uploaded = false;
		return true;
	}

	bool Texture2D::UploadToGpu() {
		if (!IsValid()) {
			m_lastError = "Texture pixel data is invalid.";
			return false;
		}

#if defined(TINYENGINE_ENABLE_OPENGL)
		if (m_textureId == 0) {
			glGenTextures(1, &m_textureId);
		}

		if (m_textureId == 0) {
			m_lastError = "glGenTextures failed.";
			return false;
		}

		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA8,
			m_width,
			m_height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			m_pixels.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		m_uploaded = true;
		return true;
#else
		m_lastError = "OpenGL backend is disabled.";
		return false;
#endif
	}

	void Texture2D::Bind(const std::uint32_t textureSlot) const {
#if defined(TINYENGINE_ENABLE_OPENGL)
		if (m_textureId != 0) {
			glActiveTexture(GL_TEXTURE0 + textureSlot);
			glBindTexture(GL_TEXTURE_2D, m_textureId);
		}
#else
		(void)textureSlot;
#endif
	}

	void Texture2D::Unbind() const {
#if defined(TINYENGINE_ENABLE_OPENGL)
		glBindTexture(GL_TEXTURE_2D, 0);
#endif
	}

	void Texture2D::Reset() {
#if defined(TINYENGINE_ENABLE_OPENGL)
		if (m_textureId != 0) {
			glDeleteTextures(1, &m_textureId);
		}
#endif
		m_pixels.clear();
		m_width = 0;
		m_height = 0;
		m_channelCount = 0;
		m_textureId = 0;
		m_uploaded = false;
	}

	bool Texture2D::IsValid() const {
		return m_width > 0 && m_height > 0 && m_channelCount > 0 && !m_pixels.empty();
	}

	bool Texture2D::IsUploaded() const {
		return m_uploaded && m_textureId != 0;
	}

	std::uint32_t Texture2D::GetTextureId() const {
		return m_textureId;
	}

	int Texture2D::GetWidth() const {
		return m_width;
	}

	int Texture2D::GetHeight() const {
		return m_height;
	}

	int Texture2D::GetChannelCount() const {
		return m_channelCount;
	}

	const std::string& Texture2D::GetLastError() const {
		return m_lastError;
	}
} // namespace TinyEngine::Graphics
