#include "TinyEngine/Math/Vector3.h"

#include <cmath>

namespace TinyEngine::Math {
	namespace {
		constexpr float kEpsilon = 1e-6f;
	}

	Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

	Vector3::Vector3(const float xValue, const float yValue, const float zValue) : x(xValue), y(yValue), z(zValue) {}

	Vector3 Vector3::operator+(const Vector3& rhs) const {
		return {x + rhs.x, y + rhs.y, z + rhs.z};
	}

	Vector3 Vector3::operator-(const Vector3& rhs) const {
		return {x - rhs.x, y - rhs.y, z - rhs.z};
	}

	Vector3 Vector3::operator*(const float scalar) const {
		return {x * scalar, y * scalar, z * scalar};
	}

	Vector3 Vector3::operator/(const float scalar) const {
		if (std::abs(scalar) < kEpsilon) {
			return {0.0f, 0.0f, 0.0f};
		}
		return {x / scalar, y / scalar, z / scalar};
	}

	Vector3& Vector3::operator+=(const Vector3& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vector3& Vector3::operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	Vector3& Vector3::operator/=(float scalar) {
		if (std::abs(scalar) < kEpsilon) {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			return *this;
		}
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	float Vector3::LengthSquared() const {
		return x * x + y * y + z * z;
	}

	float Vector3::Length() const {
		return std::sqrt(LengthSquared());
	}

	Vector3 Vector3::Normalized() const {
		const float len = Length();
		if (len < kEpsilon) {
			return {0.0f, 0.0f, 0.0f};
		}
		return {x / len, y / len, z / len};
	}

	void Vector3::Normalize() {
		const float len = Length();
		if (len < kEpsilon) {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			return;
		}
		x /= len;
		y /= len;
		z /= len;
	}

	float Vector3::Dot(const Vector3& lhs, const Vector3& rhs) {
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	Vector3 Vector3::Cross(const Vector3& lhs, const Vector3& rhs) {
		return {
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		};
	}
} // namespace TinyEngine::Math
