#pragma once

namespace TinyEngine::Math {
	struct Vector3 {
		float x;
		float y;
		float z;

		Vector3();
		Vector3(float xValue, float yValue, float zValue);

		Vector3 operator+(const Vector3& rhs) const;
		Vector3 operator-(const Vector3& rhs) const;
		Vector3 operator*(float scalar) const;
		Vector3 operator/(float scalar) const;

		Vector3& operator+=(const Vector3& rhs);
		Vector3& operator-=(const Vector3& rhs);
		Vector3& operator*=(float scalar);
		Vector3& operator/=(float scalar);

		[[nodiscard]] float LengthSquared() const;
		[[nodiscard]] float Length() const;
		[[nodiscard]] Vector3 Normalized() const;
		void Normalize();

		static float Dot(const Vector3& lhs, const Vector3& rhs);
		static Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	};
} // namespace TinyEngine::Math
