#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <system_error>

#include "TinyEngine/Graphics/Texture2D.h"
#include "TinyEngine/Resource/ResourceHandle.h"
#include "TinyEngine/Resource/ResourceManager.h"

namespace TinyEngine::Resource {
	class TextureCache {
	public:
		TextureCache() = default;

		ResourceHandle<Graphics::Texture2D> LoadTexture(std::string_view path, bool flipVertically = true) {
			const std::string normalizedPath = NormalizePath(path);
			if (normalizedPath.empty()) {
				return {};
			}

			return m_resourceManager.LoadOrGet<Graphics::Texture2D>(normalizedPath, [normalizedPath, flipVertically]() {
				auto texture = std::make_shared<Graphics::Texture2D>();
				if (!texture->LoadFromFile(normalizedPath, flipVertically)) {
					return std::shared_ptr<Graphics::Texture2D>{};
				}
				return texture;
			});
		}

		[[nodiscard]] std::shared_ptr<Graphics::Texture2D> GetTexture(std::string_view path) const {
			const std::string normalizedPath = NormalizePath(path);
			if (normalizedPath.empty()) {
				return {};
			}

			return m_resourceManager.Get<Graphics::Texture2D>(normalizedPath);
		}

		[[nodiscard]] bool Contains(std::string_view path) const {
			const std::string normalizedPath = NormalizePath(path);
			if (normalizedPath.empty()) {
				return false;
			}

			return m_resourceManager.Contains<Graphics::Texture2D>(normalizedPath);
		}

		bool UnloadTexture(std::string_view path) {
			const std::string normalizedPath = NormalizePath(path);
			if (normalizedPath.empty()) {
				return false;
			}

			return m_resourceManager.Unload<Graphics::Texture2D>(normalizedPath);
		}

		void Clear() {
			m_resourceManager.Clear();
		}

		[[nodiscard]] std::size_t Count() const {
			return m_resourceManager.Count();
		}

		[[nodiscard]] const ResourceManager& Manager() const {
			return m_resourceManager;
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
