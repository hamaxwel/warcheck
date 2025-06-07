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

    static Quaternion FromEuler(float pitch, float yaw, float roll) {
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);

        return Quaternion(
            cy * cp * sr - sy * sp * cr,
            sy * cp * sr + cy * sp * cr,
            sy * cp * cr - cy * sp * sr,
            cy * cp * cr + sy * sp * sr
        );
    }

    Quaternion operator*(const Quaternion& other) const {
        return Quaternion(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
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
            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }
    }

    Quaternion Conjugate() const {
        return Quaternion(-x, -y, -z, w);
    }

    Matrix4x4 ToMatrix() const {
        Matrix4x4 result;
        float xx = x * x;
        float xy = x * y;
        float xz = x * z;
        float xw = x * w;
        float yy = y * y;
        float yz = y * z;
        float yw = y * w;
        float zz = z * z;
        float zw = z * w;

        result.elements[0] = 1.0f - 2.0f * (yy + zz);
        result.elements[1] = 2.0f * (xy - zw);
        result.elements[2] = 2.0f * (xz + yw);
        result.elements[4] = 2.0f * (xy + zw);
        result.elements[5] = 1.0f - 2.0f * (xx + zz);
        result.elements[6] = 2.0f * (yz - xw);
        result.elements[8] = 2.0f * (xz - yw);
        result.elements[9] = 2.0f * (yz + xw);
        result.elements[10] = 1.0f - 2.0f * (xx + yy);

        return result;
    }

    Vector3 RotateVector(const Vector3& v) const {
        Quaternion qv(0.0f, v.x, v.y, v.z);
        Quaternion result = (*this) * qv * Conjugate();
        return Vector3(result.x, result.y, result.z);
    }
};

} // namespace InvasionEngine 