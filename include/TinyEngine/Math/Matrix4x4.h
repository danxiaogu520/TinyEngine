#pragma once

#include "TinyEngine/Math/Vector4.h"

namespace TinyEngine::Math {
	struct Matrix4x4 {
		float m[16];

		Matrix4x4();
		explicit Matrix4x4(float diagonal);

		float& At(int row, int col);
		const float& At(int row, int col) const;

		static Matrix4x4 Identity();
		static Matrix4x4 Translation(float tx, float ty, float tz);
		static Matrix4x4 Scale(float sx, float sy, float sz);
		static Matrix4x4 Orthographic(float left, float right, float bottom, float top, float near, float far);

		Matrix4x4 operator*(const Matrix4x4& rhs) const;
		Vector4 operator*(const Vector4& rhs) const;
	};
} // namespace TinyEngine::Math
