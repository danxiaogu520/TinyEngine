#include "TinyEngine/Math/Vector4.h"

#include <cmath>

namespace TinyEngine::Math {
	namespace {
		constexpr float kEpsilon = 1e-6f;
	}

	Vector4::Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

	Vector4::Vector4(float xValue, float yValue, float zValue, float wValue) : x(xValue), y(yValue), z(zValue), w(wValue) {}

	Vector4 Vector4::operator+(const Vector4& rhs) const {
		return {x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w};
	}

	Vector4 Vector4::operator-(const Vector4& rhs) const {
		return {x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w};
	}

	Vector4 Vector4::operator*(float scalar) const {
		return {x * scalar, y * scalar, z * scalar, w * scalar};
	}

	Vector4 Vector4::operator/(float scalar) const {
		if (std::abs(scalar) < kEpsilon) {
			return {0.0f, 0.0f, 0.0f, 0.0f};
		}
		return {x / scalar, y / scalar, z / scalar, w / scalar};
	}

	Vector4& Vector4::operator+=(const Vector4& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	Vector4& Vector4::operator-=(const Vector4& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	Vector4& Vector4::operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	Vector4& Vector4::operator/=(float scalar) {
		if (std::abs(scalar) < kEpsilon) {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
			return *this;
		}
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	float Vector4::LengthSquared() const {
		return x * x + y * y + z * z + w * w;
	}

	float Vector4::Length() const {
		return std::sqrt(LengthSquared());
	}

	Vector4 Vector4::Normalized() const {
		const float len = Length();
		if (len < kEpsilon) {
			return {0.0f, 0.0f, 0.0f, 0.0f};
		}
		return {x / len, y / len, z / len, w / len};
	}

	void Vector4::Normalize() {
		const float len = Length();
		if (len < kEpsilon) {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
			return;
		}
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}

	float Vector4::Dot(const Vector4& lhs, const Vector4& rhs) {
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}
} // namespace TinyEngine::Math
