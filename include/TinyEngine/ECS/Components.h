#pragma once

#include "TinyEngine/ECS/Component.h"

namespace TinyEngine::ECS {
	struct TransformComponent final : Component<TransformComponent> {
		constexpr TransformComponent() = default;
		constexpr TransformComponent(const float xValue, const float yValue) : x(xValue), y(yValue) {}

		float x = 0.0f;
		float y = 0.0f;
	};

	struct VelocityComponent final : Component<VelocityComponent> {
		constexpr VelocityComponent() = default;
		constexpr VelocityComponent(const float vxValue, const float vyValue) : vx(vxValue), vy(vyValue) {}

		float vx = 0.0f;
		float vy = 0.0f;
	};
} // namespace TinyEngine::ECS
