#pragma once

namespace TinyEngine::Math {

struct Vector4 {
    float x;
    float y;
    float z;
    float w;

    Vector4();
    Vector4(float xValue, float yValue, float zValue, float wValue);

    Vector4 operator+(const Vector4& rhs) const;
    Vector4 operator-(const Vector4& rhs) const;
    Vector4 operator*(float scalar) const;
    Vector4 operator/(float scalar) const;

    Vector4& operator+=(const Vector4& rhs);
    Vector4& operator-=(const Vector4& rhs);
    Vector4& operator*=(float scalar);
    Vector4& operator/=(float scalar);

    [[nodiscard]] float LengthSquared() const;
    [[nodiscard]] float Length() const;
    [[nodiscard]] Vector4 Normalized() const;
    void Normalize();

    static float Dot(const Vector4& lhs, const Vector4& rhs);
};

} // namespace TinyEngine::Math
