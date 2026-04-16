#include <cmath>
#include <iostream>

#include "TinyEngine/ECS/Components.h"
#include "TinyEngine/ECS/System.h"
#include "TinyEngine/Scene/Scene.h"

namespace {
	constexpr float kEpsilon = 1e-6f;

	bool NearlyEqual(const float lhs, const float rhs) {
		return std::fabs(lhs - rhs) <= kEpsilon;
	}

	int TestSceneEntityLifecycle() {
		TinyEngine::Scene::Scene scene;

		const TinyEngine::ECS::Entity entity = scene.CreateEntity();
		if (!scene.GetRegistry().IsAlive(entity)) return 1;

		scene.Emplace<TinyEngine::ECS::TransformComponent>(entity, 10.0f, 20.0f);
		if (!scene.Has<TinyEngine::ECS::TransformComponent>(entity)) return 1;

		if (!scene.DestroyEntity(entity)) return 1;
		if (scene.GetRegistry().IsAlive(entity)) return 1;

		return 0;
	}

	int TestSceneSystemUpdate() {
		TinyEngine::Scene::Scene scene;

		const TinyEngine::ECS::Entity player = scene.CreateEntity();
		scene.Emplace<TinyEngine::ECS::TransformComponent>(player, 0.0f, 0.0f);
		auto& controller = scene.Emplace<TinyEngine::ECS::PlayerControllerComponent>(player);
		controller.moveSpeed = 10.0f;
		controller.keyMoveRight = 'd';

		scene.AddSystem<TinyEngine::ECS::PlayerControllerSystem>([](const int keyCode) {
			return keyCode == 'd';
		});

		scene.Update(0.5);

		const auto& transform = scene.Get<TinyEngine::ECS::TransformComponent>(player);
		if (!NearlyEqual(transform.x, 5.0f) || !NearlyEqual(transform.y, 0.0f)) return 1;

		return 0;
	}

	int TestSceneClearLifecycle() {
		TinyEngine::Scene::Scene scene;

		const TinyEngine::ECS::Entity firstEntity = scene.CreateEntity();
		scene.Emplace<TinyEngine::ECS::TransformComponent>(firstEntity, 1.0f, 2.0f);
		scene.AddSystem<TinyEngine::ECS::PlayerControllerSystem>([](const int) { return false; });
		scene.Clear();

		if (scene.GetScheduler().Count() != 0) return 1;
		if (scene.GetRegistry().IsAlive(firstEntity)) return 1;

		const TinyEngine::ECS::Entity newEntity = scene.CreateEntity();
		scene.Emplace<TinyEngine::ECS::TransformComponent>(newEntity, 3.0f, 4.0f);
		const auto& transform = scene.Get<TinyEngine::ECS::TransformComponent>(newEntity);
		if (!NearlyEqual(transform.x, 3.0f) || !NearlyEqual(transform.y, 4.0f)) return 1;

		return 0;
	}
} // namespace

int main() {
	if (TestSceneEntityLifecycle() != 0) {
		std::cerr << "Scene entity lifecycle test failed" << std::endl;
		return 1;
	}

	if (TestSceneSystemUpdate() != 0) {
		std::cerr << "Scene system update test failed" << std::endl;
		return 1;
	}

	if (TestSceneClearLifecycle() != 0) {
		std::cerr << "Scene clear lifecycle test failed" << std::endl;
		return 1;
	}

	std::cout << "All scene tests passed" << std::endl;
	return 0;
}