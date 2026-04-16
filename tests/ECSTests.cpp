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
			const auto dt = static_cast<float>(deltaTimeSeconds);
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

		const auto& t1 = registry.Get<TinyEngine::ECS::TransformComponent>(e1);
		const auto& t2 = registry.Get<TinyEngine::ECS::TransformComponent>(e2);

		if (!NearlyEqual(t1.x, 1.0f) || !NearlyEqual(t1.y, 1.5f)) return 1;
		if (!NearlyEqual(t2.x, 5.0f) || !NearlyEqual(t2.y, 5.0f)) return 1;

		return 0;
	}

	int TestSparseSetStyleStorageBehavior() {
		TinyEngine::ECS::Registry registry;

		const TinyEngine::ECS::Entity e1 = registry.CreateEntity();
		const TinyEngine::ECS::Entity e2 = registry.CreateEntity();
		const TinyEngine::ECS::Entity e3 = registry.CreateEntity();

		registry.Emplace<TinyEngine::ECS::VelocityComponent>(e1, 1.0f, 2.0f);
		registry.Emplace<TinyEngine::ECS::VelocityComponent>(e2, 3.0f, 4.0f);
		registry.Emplace<TinyEngine::ECS::VelocityComponent>(e3, 5.0f, 6.0f);

		registry.Emplace<TinyEngine::ECS::VelocityComponent>(e2, 30.0f, 40.0f);
		const auto& replaced = registry.Get<TinyEngine::ECS::VelocityComponent>(e2);
		if (!NearlyEqual(replaced.vx, 30.0f) || !NearlyEqual(replaced.vy, 40.0f)) return 1;

		if (!registry.Remove<TinyEngine::ECS::VelocityComponent>(e2)) return 1;
		if (registry.Has<TinyEngine::ECS::VelocityComponent>(e2)) return 1;

		const auto entities = registry.View<TinyEngine::ECS::VelocityComponent>();
		if (entities.size() != 2) return 1;
		if (!(entities[0] == e1 || entities[0] == e3)) return 1;
		if (!(entities[1] == e1 || entities[1] == e3)) return 1;
		if (entities[0] == entities[1]) return 1;

		const auto& e1Velocity = registry.Get<TinyEngine::ECS::VelocityComponent>(e1);
		const auto& e3Velocity = registry.Get<TinyEngine::ECS::VelocityComponent>(e3);
		if (!NearlyEqual(e1Velocity.vx, 1.0f) || !NearlyEqual(e1Velocity.vy, 2.0f)) return 1;
		if (!NearlyEqual(e3Velocity.vx, 5.0f) || !NearlyEqual(e3Velocity.vy, 6.0f)) return 1;

		return 0;
	}

	int TestDay20CoreComponents() {
		TinyEngine::ECS::Registry registry;
		const TinyEngine::ECS::Entity entity = registry.CreateEntity();

		auto& transform = registry.Emplace<TinyEngine::ECS::TransformComponent>(entity, 10.0f, 20.0f);
		transform.rotationDegrees = 30.0f;
		transform.scaleX = 2.0f;
		transform.scaleY = 3.0f;

		registry.Emplace<TinyEngine::ECS::SpriteRendererComponent>(entity);
		if (!registry.Has<TinyEngine::ECS::SpriteRendererComponent>(entity)) return 1;

		const auto& sprite = registry.Get<TinyEngine::ECS::SpriteRendererComponent>(entity);
		if (!sprite.visible) return 1;
		if (!NearlyEqual(sprite.colorA, 1.0f)) return 1;
		if (!NearlyEqual(sprite.sizeX, 1.0f) || !NearlyEqual(sprite.sizeY, 1.0f)) return 1;
		if (!NearlyEqual(transform.x, 10.0f) || !NearlyEqual(transform.y, 20.0f)) return 1;
		if (!NearlyEqual(transform.scaleX, 2.0f) || !NearlyEqual(transform.scaleY, 3.0f)) return 1;

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

	if (TestSparseSetStyleStorageBehavior() != 0) {
		std::cerr << "ECS sparse set behavior tests failed" << std::endl;
		return 1;
	}

	if (TestDay20CoreComponents() != 0) {
		std::cerr << "ECS day20 core component tests failed" << std::endl;
		return 1;
	}

	std::cout << "All ECS tests passed" << std::endl;
	return 0;
}
