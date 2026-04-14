#include <cmath>
#include <iostream>

#include "TinyEngine/ECS/Components.h"
#include "TinyEngine/ECS/Registry.h"
#include "TinyEngine/ECS/System.h"

namespace {
	constexpr float kEpsilon = 1e-6f;

	bool NearlyEqual(const float lhs, const float rhs) {
		return std::fabs(lhs - rhs) <= kEpsilon;
	}

	class MovementSystem final : public TinyEngine::ECS::ISystem {
	public:
		void Update(TinyEngine::ECS::Registry& registry, const double deltaTimeSeconds) override {
			const float dt = static_cast<float>(deltaTimeSeconds);
			const auto entities = registry.View<TinyEngine::ECS::TransformComponent, TinyEngine::ECS::VelocityComponent>();
			for (const TinyEngine::ECS::Entity entity : entities) {
				auto& transform = registry.Get<TinyEngine::ECS::TransformComponent>(entity);
				const auto& velocity = registry.Get<TinyEngine::ECS::VelocityComponent>(entity);
				transform.x += velocity.vx * dt;
				transform.y += velocity.vy * dt;
			}
		}
	};

	int TestRegistryBasics() {
		TinyEngine::ECS::Registry registry;

		const TinyEngine::ECS::Entity entity = registry.CreateEntity();
		if (!registry.IsAlive(entity)) return 1;

		registry.Emplace<TinyEngine::ECS::TransformComponent>(entity, TinyEngine::ECS::TransformComponent{10.0f, 20.0f});
		if (!registry.Has<TinyEngine::ECS::TransformComponent>(entity)) return 1;

		const auto& transform = registry.Get<TinyEngine::ECS::TransformComponent>(entity);
		if (!NearlyEqual(transform.x, 10.0f) || !NearlyEqual(transform.y, 20.0f)) return 1;

		if (!registry.Remove<TinyEngine::ECS::TransformComponent>(entity)) return 1;
		if (registry.Has<TinyEngine::ECS::TransformComponent>(entity)) return 1;

		if (!registry.DestroyEntity(entity)) return 1;
		if (registry.IsAlive(entity)) return 1;

		return 0;
	}

	int TestViewAndSystemUpdate() {
		TinyEngine::ECS::Registry registry;

		const TinyEngine::ECS::Entity e1 = registry.CreateEntity();
		const TinyEngine::ECS::Entity e2 = registry.CreateEntity();

		registry.Emplace<TinyEngine::ECS::TransformComponent>(e1, TinyEngine::ECS::TransformComponent{0.0f, 0.0f});
		registry.Emplace<TinyEngine::ECS::VelocityComponent>(e1, TinyEngine::ECS::VelocityComponent{2.0f, 3.0f});

		registry.Emplace<TinyEngine::ECS::TransformComponent>(e2, TinyEngine::ECS::TransformComponent{5.0f, 5.0f});

		const auto entities = registry.View<TinyEngine::ECS::TransformComponent, TinyEngine::ECS::VelocityComponent>();
		if (entities.size() != 1 || entities[0] != e1) return 1;

		TinyEngine::ECS::SystemScheduler scheduler;
		scheduler.AddSystem<MovementSystem>();
		if (scheduler.Count() != 1) return 1;

		scheduler.Update(registry, 0.5);

		const auto& moved = registry.Get<TinyEngine::ECS::TransformComponent>(e1);
		const auto& untouched = registry.Get<TinyEngine::ECS::TransformComponent>(e2);

		if (!NearlyEqual(moved.x, 1.0f) || !NearlyEqual(moved.y, 1.5f)) return 1;
		if (!NearlyEqual(untouched.x, 5.0f) || !NearlyEqual(untouched.y, 5.0f)) return 1;

		return 0;
	}
} // namespace

int main() {
	if (TestRegistryBasics() != 0) {
		std::cerr << "ECS registry basic tests failed" << std::endl;
		return 1;
	}

	if (TestViewAndSystemUpdate() != 0) {
		std::cerr << "ECS view/system tests failed" << std::endl;
		return 1;
	}

	std::cout << "All ECS tests passed" << std::endl;
	return 0;
}
