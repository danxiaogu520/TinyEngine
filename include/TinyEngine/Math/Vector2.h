#pragma once

namespace TinyEngine::Math {

struct Vector2 {
    float x;
    float y;

    Vector2();
    Vector2(float xValue, float yValue);

    Vector2 operator+(const Vector2& rhs) const;
    Vector2 operator-(const Vector2& rhs) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;

    Vector2& operator+=(const Vector2& rhs);
    Vector2& operator-=(const Vector2& rhs);
    Vector2& operator*=(float scalar);
    Vector2& operator/=(float scalar);

    [[nodiscard]] float LengthSquared() const;
    [[nodiscard]] float Length() const;
    [[nodiscard]] Vector2 Normalized() const;
    void Normalize();

    static float Dot(const Vector2& lhs, const Vector2& rhs);
    static float Cross(const Vector2& lhs, const Vector2& rhs);
};

} // namespace TinyEngine::Math
