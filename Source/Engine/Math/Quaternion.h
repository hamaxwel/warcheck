#pragma once

#include "Vector.h"
#include "Matrix.h"
#include <cmath>

namespace InvasionEngine {

struct Quaternion {
    float x, y, z, w;

    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    static Quaternion FromAxisAngle(const Vector3& axis, float angle) {
        float halfAngle = angle * 0.5f;
        float s = std::sin(halfAngle);
        return Quaternion(
            axis.x * s,
            axis.y * s,
            axis.z * s,
            std::cos(halfAngle)
        );
    }

    static Quaternion FromEulerAngles(float pitch, float yaw, float roll) {
        return FromEuler(pitch, yaw, roll);
    }

    static Quaternion FromEuler(float pitch, float yaw, float roll) {
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        Quaternion q;
        q.w = cy * cp * cr + sy * sp * sr;
        q.x = cy * cp * sr - sy * sp * cr;
        q.y = sy * cp * sr + cy * sp * cr;
        q.z = sy * cp * cr - cy * sp * sr;
        return q;
    }

    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    Quaternion& operator*=(const Quaternion& other) {
        *this = *this * other;
        return *this;
    }

    Quaternion operator*(float scalar) const {
        return Quaternion(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    float Length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    void Normalize() {
        float len = Length();
        if (len > 0.0f) {
            float invLen = 1.0f / len;
            x *= invLen;
            y *= invLen;
            z *= invLen;
            w *= invLen;
        }
    }

    Quaternion Conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }

    Matrix4x4 ToMatrix() const {
        return Matrix4x4::FromQuaternion(*this);
    }

    Vector3 RotateVector(const Vector3& v) const {
        Quaternion qv(0.0f, v.x, v.y, v.z);
        Quaternion result = (*this) * qv * Conjugate();
        return Vector3(result.x, result.y, result.z);
    }
};

} // namespace InvasionEngine 