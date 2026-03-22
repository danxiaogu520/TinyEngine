#include "TinyEngine/Math/Vector2.h"

#include <cmath>

namespace TinyEngine::Math {
	namespace {
		constexpr float kEpsilon = 1e-6f;
	}

	Vector2::Vector2() : x(0.0f), y(0.0f) {}

	Vector2::Vector2(const float xValue, const float yValue) : x(xValue), y(yValue) {}

	Vector2 Vector2::operator+(const Vector2& rhs) const {
		return {x + rhs.x, y + rhs.y};
	}

	Vector2 Vector2::operator-(const Vector2& rhs) const {
		return {x - rhs.x, y - rhs.y};
	}

	Vector2 Vector2::operator*(const float scalar) const {
		return {x * scalar, y * scalar};
	}

	Vector2 Vector2::operator/(const float scalar) const {
		if (std::abs(scalar) < kEpsilon) {
			return {0.0f, 0.0f};
		}
		return {x / scalar, y / scalar};
	}

	Vector2& Vector2::operator+=(const Vector2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vector2& Vector2::operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& Vector2::operator/=(float scalar) {
		if (std::abs(scalar) < kEpsilon) {
			x = 0.0f;
			y = 0.0f;
			return *this;
		}
		x /= scalar;
		y /= scalar;
		return *this;
	}

	float Vector2::LengthSquared() const {
		return (x * x + y * y);
	}

	float Vector2::Length() const {
		return std::sqrt(LengthSquared());
	}

	Vector2 Vector2::Normalized() const {
		const float len = Length();
		if (len < kEpsilon) {
			return {0.0f, 0.0f};
		}
		return {x / len, y / len};
	}

	void Vector2::Normalize() {
		const float len = Length();
		if (len < kEpsilon) {
			x = 0.0f;
			y = 0.0f;
			return;
		}
		x /= len;
		y /= len;
	}

	float Vector2::Dot(const Vector2& lhs, const Vector2& rhs) {
		return lhs.x * rhs.x + lhs.y * rhs.y;
	}

	float Vector2::Cross(const Vector2& lhs, const Vector2& rhs) {
		return lhs.x * rhs.y - rhs.x * lhs.y;
	}
} // namespace TinyEngine::Math
