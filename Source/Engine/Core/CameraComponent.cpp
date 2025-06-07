#include "CameraComponent.h"
#include <cmath>

namespace InvasionEngine {

CameraComponent::CameraComponent() {}
CameraComponent::~CameraComponent() {}

void CameraComponent::SetPerspective(float fovY, float aspect, float nearZ, float farZ) {
    m_FOV = fovY;
    m_Aspect = aspect;
    m_NearZ = nearZ;
    m_FarZ = farZ;
    float f = 1.0f / std::tan(fovY * 0.5f * 3.14159265f / 180.0f);
    m_ProjectionMatrix = Matrix4x4::Identity();
    m_ProjectionMatrix.m[0][0] = f / aspect;
    m_ProjectionMatrix.m[1][1] = f;
    m_ProjectionMatrix.m[2][2] = (farZ + nearZ) / (nearZ - farZ);
    m_ProjectionMatrix.m[2][3] = (2 * farZ * nearZ) / (nearZ - farZ);
    m_ProjectionMatrix.m[3][2] = -1.0f;
    m_ProjectionMatrix.m[3][3] = 0.0f;
}

void CameraComponent::SetView(const Vector3& position, const Vector3& target, const Vector3& up) {
    Vector3 zaxis = (position - target).Normalized();
    Vector3 xaxis = up.Cross(zaxis).Normalized();
    Vector3 yaxis = zaxis.Cross(xaxis);
    m_ViewMatrix = Matrix4x4::Identity();
    m_ViewMatrix.m[0][0] = xaxis.x;
    m_ViewMatrix.m[1][0] = xaxis.y;
    m_ViewMatrix.m[2][0] = xaxis.z;
    m_ViewMatrix.m[0][1] = yaxis.x;
    m_ViewMatrix.m[1][1] = yaxis.y;
    m_ViewMatrix.m[2][1] = yaxis.z;
    m_ViewMatrix.m[0][2] = zaxis.x;
    m_ViewMatrix.m[1][2] = zaxis.y;
    m_ViewMatrix.m[2][2] = zaxis.z;
    m_ViewMatrix.m[3][0] = -xaxis.Dot(position);
    m_ViewMatrix.m[3][1] = -yaxis.Dot(position);
    m_ViewMatrix.m[3][2] = -zaxis.Dot(position);
}

const Matrix4x4& CameraComponent::GetViewMatrix() const {
    return m_ViewMatrix;
}
const Matrix4x4& CameraComponent::GetProjectionMatrix() const {
    return m_ProjectionMatrix;
}

} // namespace InvasionEngine 