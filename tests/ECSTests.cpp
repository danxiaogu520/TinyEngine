#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "TinyEngine/ECS/Components.h"
#include "TinyEngine/ECS/Registry.h"
#include "TinyEngine/ECS/System.h"
#include "TinyEngine/Resource/ResourceManager.h"

namespace {
	constexpr float kEpsilon = 1e-6f;

	bool NearlyEqual(const float lhs, const float rhs) {
		return std::fabs(lhs - rhs) <= kEpsilon;
	}

	struct DummyMaterial {
		float tint = 1.0f;
	};

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

	int TestDay21RenderSystemSubmission() {
		TinyEngine::ECS::Registry registry;

		const TinyEngine::ECS::Entity drawableA = registry.CreateEntity();
		const TinyEngine::ECS::Entity drawableB = registry.CreateEntity();
		const TinyEngine::ECS::Entity hidden = registry.CreateEntity();
		const TinyEngine::ECS::Entity noSprite = registry.CreateEntity();

		registry.Emplace<TinyEngine::ECS::TransformComponent>(drawableA, 10.0f, 20.0f);
		registry.Emplace<TinyEngine::ECS::SpriteRendererComponent>(drawableA);

		registry.Emplace<TinyEngine::ECS::TransformComponent>(drawableB, 30.0f, 40.0f);
		registry.Emplace<TinyEngine::ECS::SpriteRendererComponent>(drawableB);

		registry.Emplace<TinyEngine::ECS::TransformComponent>(hidden, 50.0f, 60.0f);
		auto& hiddenSprite = registry.Emplace<TinyEngine::ECS::SpriteRendererComponent>(hidden);
		hiddenSprite.visible = false;

		registry.Emplace<TinyEngine::ECS::TransformComponent>(noSprite, 70.0f, 80.0f);

		int submitCount = 0;
		float submittedXSum = 0.0f;
		float submittedYSum = 0.0f;

		TinyEngine::ECS::SystemScheduler scheduler;
		scheduler.AddSystem<TinyEngine::ECS::RenderSystem>(
			[&](const TinyEngine::ECS::Entity entity, const TinyEngine::ECS::TransformComponent& transform,
				const TinyEngine::ECS::SpriteRendererComponent& sprite) {
				if (!sprite.visible) return;
				if (entity != drawableA && entity != drawableB) return;

				submitCount += 1;
				submittedXSum += transform.x;
				submittedYSum += transform.y;
			});

		scheduler.Update(registry, 0.016);

		if (submitCount != 2) return 1;
		if (!NearlyEqual(submittedXSum, 40.0f)) return 1;
		if (!NearlyEqual(submittedYSum, 60.0f)) return 1;

		return 0;
	}

	int TestDay22ECSRendererResourceIntegration() {
		TinyEngine::ECS::Registry registry;
		TinyEngine::Resource::ResourceManager resourceManager;

		auto playerMaterial = std::make_shared<DummyMaterial>();
		playerMaterial->tint = 0.75f;
		auto materialHandle = resourceManager.Register<DummyMaterial>("scene/player_material", playerMaterial);
		if (!materialHandle.IsValid()) return 1;

		const TinyEngine::ECS::Entity textured = registry.CreateEntity();
		const TinyEngine::ECS::Entity untextured = registry.CreateEntity();

		registry.Emplace<TinyEngine::ECS::TransformComponent>(textured, 12.0f, 34.0f);
		auto& texturedSprite = registry.Emplace<TinyEngine::ECS::SpriteRendererComponent>(textured);
		texturedSprite.sizeX = 2.0f;
		texturedSprite.sizeY = 3.0f;

		registry.Emplace<TinyEngine::ECS::TransformComponent>(untextured, 50.0f, 60.0f);
		registry.Emplace<TinyEngine::ECS::SpriteRendererComponent>(untextured);

		int submitCount = 0;
		int materialAppliedCount = 0;
		float positionSum = 0.0f;
		std::unordered_map<TinyEngine::ECS::Entity, TinyEngine::Resource::ResourceHandle<DummyMaterial>> materialBindings;
		materialBindings[textured] = materialHandle;

		TinyEngine::ECS::SystemScheduler scheduler;
		scheduler.AddSystem<TinyEngine::ECS::RenderSystem>(
			[&](const TinyEngine::ECS::Entity entity, const TinyEngine::ECS::TransformComponent& transform,
				const TinyEngine::ECS::SpriteRendererComponent& sprite) {
				(void)entity;
				submitCount += 1;
				positionSum += transform.x + transform.y;

				auto it = materialBindings.find(entity);
				if (it != materialBindings.end()) {
					auto resolved = resourceManager.Resolve(it->second);
					if (resolved != nullptr && NearlyEqual(resolved->tint, 0.75f)) {
						materialAppliedCount += 1;
					}
				}
			});

		scheduler.Update(registry, 0.016);

		if (submitCount != 2) return 1;
		if (materialAppliedCount != 1) return 1;
		if (!NearlyEqual(positionSum, 156.0f)) return 1;

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

	if (TestDay21RenderSystemSubmission() != 0) {
		std::cerr << "ECS day21 render system tests failed" << std::endl;
		return 1;
	}

	if (TestDay22ECSRendererResourceIntegration() != 0) {
		std::cerr << "ECS day22 integration tests failed" << std::endl;
		return 1;
	}

	std::cout << "All ECS tests passed" << std::endl;
	return 0;
}
