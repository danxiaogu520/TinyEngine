#pragma once

#include <utility>

#include "TinyEngine/ECS/Component.h"
#include "TinyEngine/Resource/ResourceHandle.h"

namespace TinyEngine::Graphics {
	class Texture2D;
}

namespace TinyEngine::ECS {
	struct TransformComponent final : Component<TransformComponent> {
		constexpr TransformComponent() = default;
		constexpr TransformComponent(const float xValue, const float yValue) : x(xValue), y(yValue) {}

		float x = 0.0f;
		float y = 0.0f;
		float rotationDegrees = 0.0f;
		float scaleX = 1.0f;
		float scaleY = 1.0f;
	};

	struct VelocityComponent final : Component<VelocityComponent> {
		constexpr VelocityComponent() = default;
		constexpr VelocityComponent(const float vxValue, const float vyValue) : vx(vxValue), vy(vyValue) {}

		float vx = 0.0f;
		float vy = 0.0f;
	};

	struct SpriteRendererComponent final : Component<SpriteRendererComponent> {
		SpriteRendererComponent() = default;

		explicit SpriteRendererComponent(Resource::ResourceHandle<Graphics::Texture2D> textureHandle)
			: texture(std::move(textureHandle)) {}

		Resource::ResourceHandle<Graphics::Texture2D> texture;
		float sizeX = 1.0f;
		float sizeY = 1.0f;
		float colorR = 1.0f;
		float colorG = 1.0f;
		float colorB = 1.0f;
		float colorA = 1.0f;
		bool visible = true;
	};
} // namespace TinyEngine::ECS
