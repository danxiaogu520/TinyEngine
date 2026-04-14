#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <typeindex>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "TinyEngine/ECS/Entity.h"

namespace TinyEngine::ECS {
	class Registry {
	public:
		Registry() = default;

		Entity CreateEntity() {
			const Entity entity = m_entityIdGenerator.Next();
			if (!IsValidEntity(entity)) {
				throw std::runtime_error("Entity ID space exhausted.");
			}
			m_aliveEntities.insert(entity);
			return entity;
		}

		bool DestroyEntity(const Entity entity) {
			if (!IsAlive(entity)) {
				return false;
			}

			m_aliveEntities.erase(entity);
			for (auto& [_, pool] : m_componentPools) {
				pool->Remove(entity);
			}
			return true;
		}

		[[nodiscard]] bool IsAlive(const Entity entity) const {
			return m_aliveEntities.find(entity) != m_aliveEntities.end();
		}

		template <typename Component, typename... Args> Component& Emplace(const Entity entity, Args&&... args) {
			EnsureAlive(entity);
			auto& pool = GetOrCreatePool<Component>();
			return pool.EmplaceOrReplace(entity, std::forward<Args>(args)...);
		}

		template <typename Component> bool Remove(const Entity entity) {
			auto* pool = TryGetPool<Component>();
			if (pool == nullptr) {
				return false;
			}

			return pool->Remove(entity);
		}

		template <typename Component> [[nodiscard]] bool Has(const Entity entity) const {
			const auto* pool = TryGetPool<Component>();
			return pool != nullptr && pool->Contains(entity);
		}

		template <typename Component> Component& Get(const Entity entity) {
			auto* pool = TryGetPool<Component>();
			if (pool == nullptr || !pool->Contains(entity)) {
				throw std::runtime_error("Component not found on entity.");
			}

			return pool->Get(entity);
		}

		template <typename Component> const Component& Get(const Entity entity) const {
			const auto* pool = TryGetPool<Component>();
			if (pool == nullptr || !pool->Contains(entity)) {
				throw std::runtime_error("Component not found on entity.");
			}

			return pool->Get(entity);
		}

		template <typename... Components> [[nodiscard]] std::vector<Entity> View() const {
			if constexpr (sizeof...(Components) == 0) {
				return {};
			}

			using FirstComponent = std::tuple_element_t<0, std::tuple<Components...>>;
			const auto* firstPool = TryGetPool<FirstComponent>();
			if (firstPool == nullptr) {
				return {};
			}

			std::vector<Entity> entities;
			entities.reserve(firstPool->Entities().size());

			for (const Entity entity : firstPool->Entities()) {
				if (((Has<Components>(entity)) && ...)) {
					entities.push_back(entity);
				}
			}

			return entities;
		}

	private:
		class IComponentPool {
		public:
			virtual ~IComponentPool() = default;
			virtual bool Remove(Entity entity) = 0;
		};

		template <typename Component> class ComponentPool final : public IComponentPool {
		public:
			template <typename... Args> Component& EmplaceOrReplace(const Entity entity, Args&&... args) {
				auto it = m_sparseIndex.find(entity);
				if (it != m_sparseIndex.end()) {
					m_components[it->second] = Component{std::forward<Args>(args)...};
					return m_components[it->second];
				}

				const std::size_t index = m_entities.size();
				m_entities.push_back(entity);
				m_components.emplace_back(std::forward<Args>(args)...);
				m_sparseIndex[entity] = index;
				return m_components.back();
			}

			bool Remove(const Entity entity) override {
				auto it = m_sparseIndex.find(entity);
				if (it == m_sparseIndex.end()) {
					return false;
				}

				const std::size_t index = it->second;
				const std::size_t lastIndex = m_entities.size() - 1;
				const Entity lastEntity = m_entities.back();

				if (index != lastIndex) {
					m_entities[index] = lastEntity;
					m_components[index] = std::move(m_components.back());
					m_sparseIndex[lastEntity] = index;
				}

				m_entities.pop_back();
				m_components.pop_back();
				m_sparseIndex.erase(it);
				return true;
			}

			[[nodiscard]] bool Contains(const Entity entity) const {
				return m_sparseIndex.find(entity) != m_sparseIndex.end();
			}

			Component& Get(const Entity entity) {
				return m_components.at(m_sparseIndex.at(entity));
			}

			const Component& Get(const Entity entity) const {
				return m_components.at(m_sparseIndex.at(entity));
			}

			[[nodiscard]] const std::vector<Entity>& Entities() const {
				return m_entities;
			}

		private:
			std::vector<Entity> m_entities;
			std::vector<Component> m_components;
			std::unordered_map<Entity, std::size_t> m_sparseIndex;
		};

		void EnsureAlive(const Entity entity) const {
			if (!IsAlive(entity)) {
				throw std::runtime_error("Entity is not alive.");
			}
		}

		template <typename Component> ComponentPool<Component>& GetOrCreatePool() {
			const std::type_index type = std::type_index(typeid(Component));
			auto it = m_componentPools.find(type);
			if (it == m_componentPools.end()) {
				auto pool = std::make_unique<ComponentPool<Component>>();
				const auto [insertedIt, inserted] = m_componentPools.emplace(type, std::move(pool));
				(void)inserted;
				it = insertedIt;
			}

			return *static_cast<ComponentPool<Component>*>(it->second.get());
		}

		template <typename Component> ComponentPool<Component>* TryGetPool() {
			const std::type_index type = std::type_index(typeid(Component));
			const auto it = m_componentPools.find(type);
			if (it == m_componentPools.end()) {
				return nullptr;
			}

			return static_cast<ComponentPool<Component>*>(it->second.get());
		}

		template <typename Component> const ComponentPool<Component>* TryGetPool() const {
			const std::type_index type = std::type_index(typeid(Component));
			const auto it = m_componentPools.find(type);
			if (it == m_componentPools.end()) {
				return nullptr;
			}

			return static_cast<const ComponentPool<Component>*>(it->second.get());
		}

		EntityIdGenerator m_entityIdGenerator;
		std::unordered_set<Entity> m_aliveEntities;
		std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_componentPools;
	};
} // namespace TinyEngine::ECS
