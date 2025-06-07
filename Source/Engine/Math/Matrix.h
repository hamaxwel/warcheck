#pragma once

#include "Vector.h"
#include <array>

namespace InvasionEngine {

struct Matrix4x4 {
    std::array<float, 16> elements;

    Matrix4x4() {
        elements.fill(0.0f);
        elements[0] = elements[5] = elements[10] = elements[15] = 1.0f;
    }

    static Matrix4x4 Identity() {
        return Matrix4x4();
    }

    static Matrix4x4 Translation(const Vector3& translation) {
        Matrix4x4 result;
        result.elements[12] = translation.x;
        result.elements[13] = translation.y;
        result.elements[14] = translation.z;
        return result;
    }

    static Matrix4x4 Rotation(const Vector3& axis, float angle) {
        Matrix4x4 result;
        float c = std::cos(angle);
        float s = std::sin(angle);
        float t = 1.0f - c;
        float x = axis.x;
        float y = axis.y;
        float z = axis.z;

        result.elements[0] = t * x * x + c;
        result.elements[1] = t * x * y - z * s;
        result.elements[2] = t * x * z + y * s;
        result.elements[4] = t * x * y + z * s;
        result.elements[5] = t * y * y + c;
        result.elements[6] = t * y * z - x * s;
        result.elements[8] = t * x * z - y * s;
        result.elements[9] = t * y * z + x * s;
        result.elements[10] = t * z * z + c;

        return result;
    }

    static Matrix4x4 Scale(const Vector3& scale) {
        Matrix4x4 result;
        result.elements[0] = scale.x;
        result.elements[5] = scale.y;
        result.elements[10] = scale.z;
        return result;
    }

    static Matrix4x4 Perspective(float fov, float aspectRatio, float near, float far) {
        Matrix4x4 result;
        float tanHalfFov = std::tan(fov / 2.0f);
        
        result.elements[0] = 1.0f / (aspectRatio * tanHalfFov);
        result.elements[5] = 1.0f / tanHalfFov;
        result.elements[10] = -(far + near) / (far - near);
        result.elements[11] = -1.0f;
        result.elements[14] = -(2.0f * far * near) / (far - near);
        result.elements[15] = 0.0f;

        return result;
    }

    static Matrix4x4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
        Matrix4x4 result;
        Vector3 f = (target - eye);
        f.Normalize();
        
        Vector3 s = Vector3::Cross(f, up);
        s.Normalize();
        
        Vector3 u = Vector3::Cross(s, f);

        result.elements[0] = s.x;
        result.elements[1] = s.y;
        result.elements[2] = s.z;
        result.elements[4] = u.x;
        result.elements[5] = u.y;
        result.elements[6] = u.z;
        result.elements[8] = -f.x;
        result.elements[9] = -f.y;
        result.elements[10] = -f.z;
        result.elements[12] = -Vector3::Dot(s, eye);
        result.elements[13] = -Vector3::Dot(u, eye);
        result.elements[14] = Vector3::Dot(f, eye);

        return result;
    }

    Matrix4x4 operator*(const Matrix4x4& other) const {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                float sum = 0.0f;
                for (int k = 0; k < 4; k++) {
                    sum += elements[i * 4 + k] * other.elements[k * 4 + j];
                }
                result.elements[i * 4 + j] = sum;
            }
        }
        return result;
    }

    Vector4 operator*(const Vector4& v) const {
        Vector4 result;
        result.x = elements[0] * v.x + elements[4] * v.y + elements[8] * v.z + elements[12] * v.w;
        result.y = elements[1] * v.x + elements[5] * v.y + elements[9] * v.z + elements[13] * v.w;
        result.z = elements[2] * v.x + elements[6] * v.y + elements[10] * v.z + elements[14] * v.w;
        result.w = elements[3] * v.x + elements[7] * v.y + elements[11] * v.z + elements[15] * v.w;
        return result;
    }
};

} // namespace InvasionEngine 