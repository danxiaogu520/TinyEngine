#include <cmath>
#include <iostream>

#include "TinyEngine/Graphics/OrthographicCamera.h"
#include "TinyEngine/Math/Matrix4x4.h"
#include "TinyEngine/Math/Vector2.h"
#include "TinyEngine/Math/Vector3.h"
#include "TinyEngine/Math/Vector4.h"

namespace {
	constexpr float kEpsilon = 1e-4f;

	bool NearlyEqual(const float lhs, const float rhs) {
		return std::fabs(lhs - rhs) <= kEpsilon;
	}

	int TestVector2() {
		const TinyEngine::Math::Vector2 a{3.0f, 4.0f};
		const TinyEngine::Math::Vector2 b{1.0f, 2.0f};

		if (!NearlyEqual((a + b).x, 4.0f) || !NearlyEqual((a + b).y, 6.0f)) return 1;
		if (!NearlyEqual((a - b).x, 2.0f) || !NearlyEqual((a - b).y, 2.0f)) return 1;
		if (!NearlyEqual(TinyEngine::Math::Vector2::Dot(a, b), 11.0f)) return 1;
		if (!NearlyEqual(TinyEngine::Math::Vector2::Cross(a, b), 2.0f)) return 1;
		if (!NearlyEqual(a.Length(), 5.0f)) return 1;

		const TinyEngine::Math::Vector2 n = a.Normalized();
		if (!NearlyEqual(n.Length(), 1.0f)) return 1;

		return 0;
	}

	int TestVector3() {
		const TinyEngine::Math::Vector3 x{1.0f, 0.0f, 0.0f};
		const TinyEngine::Math::Vector3 y{0.0f, 1.0f, 0.0f};

		const TinyEngine::Math::Vector3 z = TinyEngine::Math::Vector3::Cross(x, y);
		if (!NearlyEqual(z.x, 0.0f) || !NearlyEqual(z.y, 0.0f) || !NearlyEqual(z.z, 1.0f)) return 1;
		if (!NearlyEqual(TinyEngine::Math::Vector3::Dot(x, y), 0.0f)) return 1;

		return 0;
	}

	int TestVector4() {
		const TinyEngine::Math::Vector4 a{1.0f, 2.0f, 3.0f, 1.0f};
		const TinyEngine::Math::Vector4 b{2.0f, 3.0f, 4.0f, 1.0f};

		if (!NearlyEqual(TinyEngine::Math::Vector4::Dot(a, b), 21.0f)) return 1;

		const TinyEngine::Math::Vector4 n = a.Normalized();
		if (!NearlyEqual(n.Length(), 1.0f)) return 1;

		return 0;
	}

	int TestMatrix4x4() {
		const TinyEngine::Math::Matrix4x4 t = TinyEngine::Math::Matrix4x4::Translation(2.0f, 3.0f, 4.0f);
		const TinyEngine::Math::Vector4 p{1.0f, 1.0f, 1.0f, 1.0f};
		const TinyEngine::Math::Vector4 moved = t * p;

		if (!NearlyEqual(moved.x, 3.0f) || !NearlyEqual(moved.y, 4.0f) || !NearlyEqual(moved.z, 5.0f) || !NearlyEqual(moved.w, 1.0f)) return 1;

		const TinyEngine::Math::Matrix4x4 s = TinyEngine::Math::Matrix4x4::Scale(2.0f, 3.0f, 4.0f);
		const TinyEngine::Math::Vector4 scaled = s * p;
		if (!NearlyEqual(scaled.x, 2.0f) || !NearlyEqual(scaled.y, 3.0f) || !NearlyEqual(scaled.z, 4.0f) || !NearlyEqual(scaled.w, 1.0f)) return 1;

		const TinyEngine::Math::Matrix4x4 ortho = TinyEngine::Math::Matrix4x4::Orthographic(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
		if (!NearlyEqual(ortho.At(0, 0), 1.0f)) return 1;
		if (!NearlyEqual(ortho.At(1, 1), 1.0f)) return 1;

		return 0;
	}

	int TestOrthographicCamera() {
		TinyEngine::Graphics::OrthographicCamera camera(-10.0f, 10.0f, -5.0f, 5.0f, -1.0f, 1.0f);

		const TinyEngine::Math::Matrix4x4& projection = camera.GetProjectionMatrix();
		if (!NearlyEqual(projection.At(0, 0), 0.1f)) return 1;
		if (!NearlyEqual(projection.At(1, 1), 0.2f)) return 1;

		camera.SetPosition(2.0f, 3.0f, 0.0f);
		const TinyEngine::Math::Matrix4x4& view = camera.GetViewMatrix();
		if (!NearlyEqual(view.At(0, 3), -2.0f)) return 1;
		if (!NearlyEqual(view.At(1, 3), -3.0f)) return 1;

		camera.SetRotationZRadians(3.1415926535f * 0.5f);
		const TinyEngine::Math::Matrix4x4& rotatedView = camera.GetViewMatrix();
		if (!NearlyEqual(rotatedView.At(0, 0), 0.0f)) return 1;

		const TinyEngine::Math::Matrix4x4& vp = camera.GetViewProjectionMatrix();
		if (!NearlyEqual(vp.At(3, 3), 1.0f)) return 1;

		return 0;
	}
} // namespace

int main() {
	if (TestVector2() != 0) {
		std::cerr << "Vector2 tests failed" << std::endl;
		return 1;
	}

	if (TestVector3() != 0) {
		std::cerr << "Vector3 tests failed" << std::endl;
		return 1;
	}

	if (TestVector4() != 0) {
		std::cerr << "Vector4 tests failed" << std::endl;
		return 1;
	}

	if (TestMatrix4x4() != 0) {
		std::cerr << "Matrix4x4 tests failed" << std::endl;
		return 1;
	}

	if (TestOrthographicCamera() != 0) {
		std::cerr << "OrthographicCamera tests failed" << std::endl;
		return 1;
	}

	std::cout << "All math tests passed" << std::endl;
	return 0;
}
