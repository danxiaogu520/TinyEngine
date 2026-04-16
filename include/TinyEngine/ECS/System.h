#pragma once

#include <cmath>
#include <cstddef>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "TinyEngine/ECS/Components.h"
#include "TinyEngine/ECS/Registry.h"

namespace TinyEngine::ECS {
	class ISystem {
	public:
		virtual ~ISystem() = default;
		virtual void Update(Registry& registry, double deltaTimeSeconds) = 0;
	};

	class PlayerControllerSystem final : public ISystem {
	public:
		using IsKeyDownCallback = std::function<bool(int keyCode)>;

		explicit PlayerControllerSystem(IsKeyDownCallback isKeyDown)
			: m_isKeyDown(std::move(isKeyDown)) {}

		void SetIsKeyDownCallback(IsKeyDownCallback isKeyDown) {
			m_isKeyDown = std::move(isKeyDown);
		}

		void Update(Registry& registry, const double deltaTimeSeconds) override {
			if (!m_isKeyDown) {
				return;
			}

			const float dt = static_cast<float>(deltaTimeSeconds);
			const auto entities = registry.View<TransformComponent, PlayerControllerComponent>();
			for (const Entity entity : entities) {
				auto& transform = registry.Get<TransformComponent>(entity);
				const auto& controller = registry.Get<PlayerControllerComponent>(entity);
				if (!controller.enabled) {
					continue;
				}

				float axisX = 0.0f;
				float axisY = 0.0f;
				if (m_isKeyDown(controller.keyMoveRight)) {
					axisX += 1.0f;
				}
				if (m_isKeyDown(controller.keyMoveLeft)) {
					axisX -= 1.0f;
				}
				if (m_isKeyDown(controller.keyMoveDown)) {
					axisY += 1.0f;
				}
				if (m_isKeyDown(controller.keyMoveUp)) {
					axisY -= 1.0f;
				}

				const float lengthSquared = axisX * axisX + axisY * axisY;
				if (lengthSquared <= 0.0f) {
					continue;
				}

				const float invLength = 1.0f / std::sqrt(lengthSquared);
				axisX *= invLength;
				axisY *= invLength;

				transform.x += axisX * controller.moveSpeed * dt;
				transform.y += axisY * controller.moveSpeed * dt;
			}
		}

	private:
		IsKeyDownCallback m_isKeyDown;
	};

	class RenderSystem final : public ISystem {
	public:
		using SubmitCallback = std::function<void(Entity entity, const TransformComponent& transform, const SpriteRendererComponent& sprite)>;

		RenderSystem() = default;

		explicit RenderSystem(SubmitCallback submitCallback)
			: m_submitCallback(std::move(submitCallback)) {}

		void SetSubmitCallback(SubmitCallback submitCallback) {
			m_submitCallback = std::move(submitCallback);
		}

		void Update(Registry& registry, const double deltaTimeSeconds) override {
			(void)deltaTimeSeconds;
			if (!m_submitCallback) {
				return;
			}

			const auto entities = registry.View<TransformComponent, SpriteRendererComponent>();
			for (const Entity entity : entities) {
				const auto& transform = registry.Get<TransformComponent>(entity);
				const auto& sprite = registry.Get<SpriteRendererComponent>(entity);
				if (!sprite.visible) {
					continue;
				}

				m_submitCallback(entity, transform, sprite);
			}
		}

	private:
		SubmitCallback m_submitCallback;
	};

	class SystemScheduler {
	public:
		template <typename SystemType, typename... Args> SystemType& AddSystem(Args&&... args) {
			static_assert(std::is_base_of_v<ISystem, SystemType>, "SystemType must derive from ISystem.");

			auto system = std::make_unique<SystemType>(std::forward<Args>(args)...);
			SystemType* rawPointer = system.get();
			m_systems.push_back(std::move(system));
			return *rawPointer;
		}

		void Update(Registry& registry, const double deltaTimeSeconds) {
			for (auto& system : m_systems) {
				system->Update(registry, deltaTimeSeconds);
			}
		}

		[[nodiscard]] std::size_t Count() const {
			return m_systems.size();
		}

	private:
		std::vector<std::unique_ptr<ISystem>> m_systems;
	};
} // namespace TinyEngine::ECS
