#include "TinyEngine/Resource/AudioClip.h"

#include <cstring>

namespace TinyEngine::Resource {
	bool AudioClip::LoadFromWavFile(std::string_view filePath) {
		Reset();
		m_lastError.clear();

		SDL_AudioSpec spec{};
		std::uint8_t* rawBuffer = nullptr;
		std::uint32_t rawLength = 0;

		if (SDL_LoadWAV(std::string(filePath).c_str(), &spec, &rawBuffer, &rawLength) == nullptr) {
			m_lastError = "Failed to load WAV file.";
			return false;
		}

		if (rawBuffer == nullptr || rawLength == 0 || spec.channels == 0 || spec.freq <= 0) {
			if (rawBuffer != nullptr) {
				SDL_FreeWAV(rawBuffer);
			}
			m_lastError = "WAV file data is invalid.";
			return false;
		}

		m_data.resize(rawLength);
		std::memcpy(m_data.data(), rawBuffer, rawLength);
		SDL_FreeWAV(rawBuffer);

		m_sampleRate = spec.freq;
		m_channels = static_cast<std::uint16_t>(spec.channels);
		m_format = spec.format;
		return true;
	}

	void AudioClip::Reset() {
		m_data.clear();
		m_sampleRate = 0;
		m_channels = 0;
		m_format = 0;
	}

	bool AudioClip::IsValid() const {
		return m_sampleRate > 0 && m_channels > 0 && m_format != 0 && !m_data.empty();
	}

	int AudioClip::SampleRate() const {
		return m_sampleRate;
	}

	std::uint16_t AudioClip::Channels() const {
		return m_channels;
	}

	SDL_AudioFormat AudioClip::Format() const {
		return m_format;
	}

	const std::vector<std::uint8_t>& AudioClip::Data() const {
		return m_data;
	}

	const std::string& AudioClip::LastError() const {
		return m_lastError;
	}
} // namespace TinyEngine::Resource
