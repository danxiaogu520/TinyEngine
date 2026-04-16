#pragma once

#include <utility>

#include "TinyEngine/ECS/Registry.h"
#include "TinyEngine/ECS/System.h"

namespace TinyEngine::Scene {
	class Scene {
	public:
		Scene() = default;

		ECS::Entity CreateEntity() {
			return m_registry.CreateEntity();
		}

		bool DestroyEntity(const ECS::Entity entity) {
			return m_registry.DestroyEntity(entity);
		}

		template <typename Component, typename... Args> Component& Emplace(const ECS::Entity entity, Args&&... args) {
			return m_registry.Emplace<Component>(entity, std::forward<Args>(args)...);
		}

		template <typename Component> bool Remove(const ECS::Entity entity) {
			return m_registry.Remove<Component>(entity);
		}

		template <typename Component> [[nodiscard]] bool Has(const ECS::Entity entity) const {
			return m_registry.Has<Component>(entity);
		}

		template <typename Component> Component& Get(const ECS::Entity entity) {
			return m_registry.Get<Component>(entity);
		}

		template <typename Component> const Component& Get(const ECS::Entity entity) const {
			return m_registry.Get<Component>(entity);
		}

		template <typename SystemType, typename... Args> SystemType& AddSystem(Args&&... args) {
			return m_scheduler.AddSystem<SystemType>(std::forward<Args>(args)...);
		}

		void Update(const double deltaTimeSeconds) {
			m_scheduler.Update(m_registry, deltaTimeSeconds);
		}

		void Clear() {
			m_scheduler = ECS::SystemScheduler();
			m_registry = ECS::Registry();
		}

		[[nodiscard]] ECS::Registry& GetRegistry() {
			return m_registry;
		}

		[[nodiscard]] const ECS::Registry& GetRegistry() const {
			return m_registry;
		}

		[[nodiscard]] ECS::SystemScheduler& GetScheduler() {
			return m_scheduler;
		}

		[[nodiscard]] const ECS::SystemScheduler& GetScheduler() const {
			return m_scheduler;
		}

	private:
		ECS::Registry m_registry;
		ECS::SystemScheduler m_scheduler;
	};
} // namespace TinyEngine::Scene