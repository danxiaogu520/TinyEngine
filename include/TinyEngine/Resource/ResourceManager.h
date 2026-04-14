#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <typeindex>
#include <unordered_map>
#include <utility>

#include "TinyEngine/Resource/ResourceHandle.h"

namespace TinyEngine::Resource {
	class ResourceManager {
	public:
		ResourceManager() = default;

		template <typename TResource>
		ResourceHandle<TResource> Register(std::string key, std::shared_ptr<TResource> resource) {
			if (key.empty() || resource == nullptr) {
				return {};
			}

			const std::string typedKey = BuildTypedKey<TResource>(key);
			auto found = m_entriesByTypedKey.find(typedKey);
			if (found == m_entriesByTypedKey.end()) {
				const std::uint64_t id = m_nextId++;
				Entry entry;
				entry.id = id;
				entry.key = std::move(key);
				entry.type = std::type_index(typeid(TResource));
				entry.resource = std::move(resource);
				m_entriesByTypedKey.emplace(typedKey, std::move(entry));
			} else {
				found->second.resource = std::move(resource);
			}

			const auto& entry = m_entriesByTypedKey.at(typedKey);
			auto typedResource = std::static_pointer_cast<TResource>(entry.resource);
			return ResourceHandle<TResource>(entry.id, entry.key, typedResource);
		}

		template <typename TResource>
		[[nodiscard]] ResourceHandle<TResource> GetHandle(std::string_view key) const {
			const std::string typedKey = BuildTypedKey<TResource>(key);
			const auto found = m_entriesByTypedKey.find(typedKey);
			if (found == m_entriesByTypedKey.end()) {
				return {};
			}

			auto typedResource = std::static_pointer_cast<TResource>(found->second.resource);
			return ResourceHandle<TResource>(found->second.id, found->second.key, typedResource);
		}

		template <typename TResource>
		[[nodiscard]] std::shared_ptr<TResource> Get(std::string_view key) const {
			return GetHandle<TResource>(key).Lock();
		}

		template <typename TResource, typename TFactory>
		ResourceHandle<TResource> LoadOrGet(std::string key, TFactory&& factory) {
			if (key.empty()) {
				return {};
			}

			if (auto existing = GetHandle<TResource>(key); existing.IsValid()) {
				return existing;
			}

			auto resource = std::forward<TFactory>(factory)();
			if (resource == nullptr) {
				return {};
			}

			return Register<TResource>(std::move(key), std::move(resource));
		}

		template <typename TResource>
		[[nodiscard]] std::shared_ptr<TResource> Resolve(const ResourceHandle<TResource>& handle) const {
			return handle.Lock();
		}

		template <typename TResource>
		[[nodiscard]] bool Contains(const std::string_view key) const {
			const std::string typedKey = BuildTypedKey<TResource>(key);
			return m_entriesByTypedKey.find(typedKey) != m_entriesByTypedKey.end();
		}

		template <typename TResource>
		bool Unload(const std::string_view key) {
			const std::string typedKey = BuildTypedKey<TResource>(key);
			return m_entriesByTypedKey.erase(typedKey) > 0;
		}

		void Clear() {
			m_entriesByTypedKey.clear();
		}

		[[nodiscard]] std::size_t Count() const {
			return m_entriesByTypedKey.size();
		}

	private:
		struct Entry {
			std::uint64_t id = 0;
			std::string key;
			std::type_index type = std::type_index(typeid(void));
			std::shared_ptr<void> resource;
		};

		template <typename TResource>
		static std::string BuildTypedKey(std::string_view key) {
			std::string typedKey = typeid(TResource).name();
			typedKey.push_back(':');
			typedKey.append(key.begin(), key.end());
			return typedKey;
		}

		std::uint64_t m_nextId = 1;
		std::unordered_map<std::string, Entry> m_entriesByTypedKey;
	};
} // namespace TinyEngine::Resource
