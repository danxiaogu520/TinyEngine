#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <system_error>

#include "TinyEngine/Resource/AudioClip.h"
#include "TinyEngine/Resource/ResourceHandle.h"
#include "TinyEngine/Resource/ResourceManager.h"

namespace TinyEngine::Resource {
	class AudioCache {
	public:
		AudioCache() = default;

		ResourceHandle<AudioClip> LoadAudio(std::string_view path) {
			const std::string normalizedPath = NormalizePath(path);
			if (normalizedPath.empty()) {
				return {};
			}

			return m_resourceManager.LoadOrGet<AudioClip>(normalizedPath, [normalizedPath]() {
				auto clip = std::make_shared<AudioClip>();
				if (!clip->LoadFromWavFile(normalizedPath)) {
					return std::shared_ptr<AudioClip>{};
				}
				return clip;
			});
		}

		[[nodiscard]] std::shared_ptr<AudioClip> GetAudio(std::string_view path) const {
			const std::string normalizedPath = NormalizePath(path);
			if (normalizedPath.empty()) {
				return {};
			}

			return m_resourceManager.Get<AudioClip>(normalizedPath);
		}

		[[nodiscard]] bool Contains(std::string_view path) const {
			const std::string normalizedPath = NormalizePath(path);
			if (normalizedPath.empty()) {
				return false;
			}

			return m_resourceManager.Contains<AudioClip>(normalizedPath);
		}

		bool UnloadAudio(std::string_view path) {
			const std::string normalizedPath = NormalizePath(path);
			if (normalizedPath.empty()) {
				return false;
			}

			return m_resourceManager.Unload<AudioClip>(normalizedPath);
		}

		void Clear() {
			m_resourceManager.Clear();
		}

		[[nodiscard]] std::size_t Count() const {
			return m_resourceManager.Count();
		}

	private:
		static std::string NormalizePath(std::string_view path) {
			if (path.empty()) {
				return {};
			}

			std::error_code errorCode;
			const std::filesystem::path raw(path);
			const std::filesystem::path absolute = std::filesystem::absolute(raw, errorCode);
			if (errorCode) {
				return raw.lexically_normal().string();
			}

			const std::filesystem::path canonicalized = std::filesystem::weakly_canonical(absolute, errorCode);
			if (errorCode) {
				return absolute.lexically_normal().string();
			}

			return canonicalized.string();
		}

		ResourceManager m_resourceManager;
	};
} // namespace TinyEngine::Resource
