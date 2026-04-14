#pragma once

#include "TinyEngine/Math/Matrix4x4.h"
#include "TinyEngine/Math/Vector3.h"

namespace TinyEngine::Graphics {
	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);

		void SetProjection(float left, float right, float bottom, float top, float nearPlane = -1.0f, float farPlane = 1.0f);
		void SetPosition(const TinyEngine::Math::Vector3& position);
		void SetPosition(float x, float y, float z);
		void SetRotationZRadians(float radians);

		[[nodiscard]] const TinyEngine::Math::Vector3& GetPosition() const;
		[[nodiscard]] float GetRotationZRadians() const;
		[[nodiscard]] const TinyEngine::Math::Matrix4x4& GetProjectionMatrix() const;
		[[nodiscard]] const TinyEngine::Math::Matrix4x4& GetViewMatrix() const;
		[[nodiscard]] const TinyEngine::Math::Matrix4x4& GetViewProjectionMatrix() const;

	private:
		void RecalculateViewProjection();

		TinyEngine::Math::Matrix4x4 m_projectionMatrix;
		TinyEngine::Math::Matrix4x4 m_viewMatrix;
		TinyEngine::Math::Matrix4x4 m_viewProjectionMatrix;
		TinyEngine::Math::Vector3 m_position;
		float m_rotationZRadians = 0.0f;
	};
} // namespace TinyEngine::Graphics
