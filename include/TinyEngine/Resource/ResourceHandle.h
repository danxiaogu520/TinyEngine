#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace TinyEngine::Resource {
	class ResourceManager;

	template <typename TResource> class ResourceHandle {
	public:
		ResourceHandle() = default;

		[[nodiscard]] bool IsValid() const {
			return m_id != 0 && !m_resource.expired();
		}

		explicit operator bool() const {
			return IsValid();
		}

		[[nodiscard]] std::uint64_t Id() const {
			return m_id;
		}

		[[nodiscard]] const std::string& Key() const {
			return m_key;
		}

		[[nodiscard]] std::shared_ptr<TResource> Lock() const {
			return m_resource.lock();
		}

	private:
		friend class ResourceManager;

		ResourceHandle(std::uint64_t id, std::string key, std::weak_ptr<TResource> resource) : m_id(id), m_key(std::move(key)), m_resource(std::move(resource)) {}

		std::uint64_t m_id = 0;
		std::string m_key;
		std::weak_ptr<TResource> m_resource;
	};
} // namespace TinyEngine::Resource
