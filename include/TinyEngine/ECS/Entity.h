#pragma once

#include <cstdint>
#include <limits>

namespace TinyEngine::ECS {
	using EntityId = std::uint32_t;
	using Entity = EntityId;
	constexpr Entity kInvalidEntity = 0;

	class EntityIdGenerator {
	public:
		EntityIdGenerator() = default;

		Entity Next() {
			if (m_next == std::numeric_limits<EntityId>::max()) {
				return kInvalidEntity;
			}

			return m_next++;
		}

	private:
		EntityId m_next = kInvalidEntity + 1;
	};

	[[nodiscard]] constexpr bool IsValidEntity(const Entity entity) {
		return entity != kInvalidEntity;
	}
} // namespace TinyEngine::ECS
