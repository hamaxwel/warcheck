#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace InvasionEngine {

using Matrix4 = glm::mat4;
using Vector2 = glm::vec2;
using Vector3 = glm::vec3;
using Vector4 = glm::vec4;
using Quaternion = glm::quat;

inline Matrix4 CreateTranslationMatrix(const Vector3& translation) {
    return glm::translate(Matrix4(1.0f), translation);
}

inline Matrix4 CreateRotationMatrix(const Quaternion& rotation) {
    return glm::toMat4(rotation);
}

inline Matrix4 CreateScaleMatrix(const Vector3& scale) {
    return glm::scale(Matrix4(1.0f), scale);
}

inline Matrix4 CreateLookAtMatrix(const Vector3& eye, const Vector3& center, const Vector3& up) {
    return glm::lookAt(eye, center, up);
}

inline Matrix4 CreatePerspectiveMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

inline Matrix4 CreateOrthographicMatrix(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    return glm::ortho(left, right, bottom, top, nearPlane, farPlane);
}

inline Vector3 Normalize(const Vector3& v) {
    return glm::normalize(v);
}

inline float Dot(const Vector3& a, const Vector3& b) {
    return glm::dot(a, b);
}

inline Vector3 Cross(const Vector3& a, const Vector3& b) {
    return glm::cross(a, b);
}

inline float Length(const Vector3& v) {
    return glm::length(v);
}

inline float Distance(const Vector3& a, const Vector3& b) {
    return glm::distance(a, b);
}

inline Quaternion CreateQuaternionFromEuler(float pitch, float yaw, float roll) {
    return glm::quat(glm::vec3(pitch, yaw, roll));
}

inline Quaternion CreateQuaternionFromAxisAngle(const Vector3& axis, float angle) {
    return glm::angleAxis(angle, axis);
}

inline Vector3 RotateVector(const Vector3& v, const Quaternion& q) {
    return glm::rotate(q, v);
}

} // namespace InvasionEngine 