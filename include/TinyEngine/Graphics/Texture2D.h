#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace TinyEngine::Graphics {
	class Texture2D {
	public:
		Texture2D() = default;
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;

		Texture2D(Texture2D&& other) noexcept;
		Texture2D& operator=(Texture2D&& other) noexcept;

		bool LoadFromFile(std::string_view filePath, bool flipVertically = true);
		bool UploadToGpu();

		void Bind(std::uint32_t textureSlot = 0) const;
		void Unbind() const;

		void Reset();

		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] bool IsUploaded() const;
		[[nodiscard]] std::uint32_t GetTextureId() const;
		[[nodiscard]] int GetWidth() const;
		[[nodiscard]] int GetHeight() const;
		[[nodiscard]] int GetChannelCount() const;
		[[nodiscard]] const std::string& GetLastError() const;

	private:
		std::vector<unsigned char> m_pixels;
		int m_width = 0;
		int m_height = 0;
		int m_channelCount = 0;
		std::uint32_t m_textureId = 0;
		bool m_uploaded = false;
		std::string m_lastError;
	};
} // namespace TinyEngine::Graphics
