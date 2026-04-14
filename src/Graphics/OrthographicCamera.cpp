#include "TinyEngine/Graphics/OrthographicCamera.h"

#include <cmath>

namespace TinyEngine::Graphics {
	namespace {
		Math::Matrix4x4 RotationZ(const float radians) {
			Math::Matrix4x4 rotation = Math::Matrix4x4::Identity();
			const float c = std::cos(radians);
			const float s = std::sin(radians);

			rotation.At(0, 0) = c;
			rotation.At(0, 1) = -s;
			rotation.At(1, 0) = s;
			rotation.At(1, 1) = c;
			return rotation;
		}
	} // namespace

	OrthographicCamera::OrthographicCamera(
		const float left,
		const float right,
		const float bottom,
		const float top,
		const float nearPlane,
		const float farPlane)
		: m_projectionMatrix(Math::Matrix4x4::Orthographic(left, right, bottom, top, nearPlane, farPlane)),
		  m_viewMatrix(Math::Matrix4x4::Identity()),
		  m_viewProjectionMatrix(Math::Matrix4x4::Identity()),
		  m_position(0.0f, 0.0f, 0.0f) {
		RecalculateViewProjection();
	}

	void OrthographicCamera::SetProjection(
		const float left,
		const float right,
		const float bottom,
		const float top,
		const float nearPlane,
		const float farPlane) {
		m_projectionMatrix = Math::Matrix4x4::Orthographic(left, right, bottom, top, nearPlane, farPlane);
		RecalculateViewProjection();
	}

	void OrthographicCamera::SetPosition(const Math::Vector3& position) {
		m_position = position;
		RecalculateViewProjection();
	}

	void OrthographicCamera::SetPosition(const float x, const float y, const float z) {
		m_position = Math::Vector3(x, y, z);
		RecalculateViewProjection();
	}

	void OrthographicCamera::SetRotationZRadians(const float radians) {
		m_rotationZRadians = radians;
		RecalculateViewProjection();
	}

	const Math::Vector3& OrthographicCamera::GetPosition() const {
		return m_position;
	}

	float OrthographicCamera::GetRotationZRadians() const {
		return m_rotationZRadians;
	}

	const Math::Matrix4x4& OrthographicCamera::GetProjectionMatrix() const {
		return m_projectionMatrix;
	}

	const Math::Matrix4x4& OrthographicCamera::GetViewMatrix() const {
		return m_viewMatrix;
	}

	const Math::Matrix4x4& OrthographicCamera::GetViewProjectionMatrix() const {
		return m_viewProjectionMatrix;
	}

	void OrthographicCamera::RecalculateViewProjection() {
		const Math::Matrix4x4 inverseRotation = RotationZ(-m_rotationZRadians);
		const Math::Matrix4x4 inverseTranslation = Math::Matrix4x4::Translation(-m_position.x, -m_position.y, -m_position.z);
		m_viewMatrix = inverseRotation * inverseTranslation;
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}
} // namespace Graphics
