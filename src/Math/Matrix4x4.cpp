#include "TinyEngine/Math/Matrix4x4.h"

namespace TinyEngine::Math {
	Matrix4x4::Matrix4x4() : m{
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	} {}

	Matrix4x4::Matrix4x4(const float diagonal) : Matrix4x4() {
		m[0] = diagonal;
		m[5] = diagonal;
		m[10] = diagonal;
		m[15] = diagonal;
	}

	float& Matrix4x4::At(const int row, const int col) {
		return m[row * 4 + col];
	}

	const float& Matrix4x4::At(const int row, const int col) const {
		return m[row * 4 + col];
	}

	Matrix4x4 Matrix4x4::Identity() {
		return Matrix4x4(1.0f);
	}

	Matrix4x4 Matrix4x4::Translation(const float tx, const float ty, const float tz) {
		auto matrix = Identity();
		matrix.At(0, 3) = tx;
		matrix.At(1, 3) = ty;
		matrix.At(2, 3) = tz;
		return matrix;
	}

	Matrix4x4 Matrix4x4::Scale(const float sx, const float sy, const float sz) {
		auto matrix = Identity();
		matrix.At(0, 0) = sx;
		matrix.At(1, 1) = sy;
		matrix.At(2, 2) = sz;
		return matrix;
	}

	Matrix4x4 Matrix4x4::Orthographic(const float left, const float right,
									  const float bottom, const float top,
									  const float near, const float far) {
		auto matrix = Identity();
		matrix.At(0, 0) = 2.0f / (right - left);
		matrix.At(1, 1) = 2.0f / (top - bottom);
		matrix.At(2, 2) = -2.0f / (far - near);

		matrix.At(0, 3) = -(right + left) / (right - left);
		matrix.At(1, 3) = -(top + bottom) / (top - bottom);
		matrix.At(2, 3) = -(far + near) / (far - near);
		return matrix;
	}

	Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const {
		auto matrix = Identity();
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				matrix.At(i, j) = At(i, 0) * rhs.At(0, j) +
					At(i, 1) * rhs.At(1, j) +
					At(i, 2) * rhs.At(2, j) +
					At(i, 3) * rhs.At(3, j);
			}
		}
		return matrix;
	}

	Vector4 Matrix4x4::operator*(const Vector4& rhs) const {
		return {
			At(0, 0) * rhs.x + At(0, 1) * rhs.y + At(0, 2) * rhs.z + At(0, 3) * rhs.w,
			At(1, 0) * rhs.x + At(1, 1) * rhs.y + At(1, 2) * rhs.z + At(1, 3) * rhs.w,
			At(2, 0) * rhs.x + At(2, 1) * rhs.y + At(2, 2) * rhs.z + At(2, 3) * rhs.w,
			At(3, 0) * rhs.x + At(3, 1) * rhs.y + At(3, 2) * rhs.z + At(3, 3) * rhs.w,
		};
	}
} // namespace TinyEngine::Math
