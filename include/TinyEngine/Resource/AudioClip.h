#pragma once

#include <SDL_audio.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace TinyEngine::Resource {
	class AudioClip {
	public:
		AudioClip() = default;

		[[nodiscard]] bool LoadFromWavFile(std::string_view filePath);
		void Reset();

		[[nodiscard]] bool IsValid() const;
		[[nodiscard]] int SampleRate() const;
		[[nodiscard]] std::uint16_t Channels() const;
		[[nodiscard]] SDL_AudioFormat Format() const;
		[[nodiscard]] const std::vector<std::uint8_t>& Data() const;
		[[nodiscard]] const std::string& LastError() const;

	private:
		std::vector<std::uint8_t> m_data;
		int m_sampleRate = 0;
		std::uint16_t m_channels = 0;
		SDL_AudioFormat m_format = 0;
		std::string m_lastError;
	};
} // namespace TinyEngine::Resource
