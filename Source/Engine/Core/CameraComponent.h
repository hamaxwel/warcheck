#pragma once

#include "Component.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace InvasionEngine {

class CameraComponent : public Component {
public:
    CameraComponent();
    virtual ~CameraComponent();

    void SetPerspective(float fovY, float aspect, float nearZ, float farZ);
    void SetView(const Vector3& position, const Vector3& target, const Vector3& up);

    const Matrix4x4& GetViewMatrix() const;
    const Matrix4x4& GetProjectionMatrix() const;

    float GetFOV() const { return m_FOV; }
    float GetAspect() const { return m_Aspect; }
    float GetNearZ() const { return m_NearZ; }
    float GetFarZ() const { return m_FarZ; }

private:
    float m_FOV = 60.0f;
    float m_Aspect = 1.777f;
    float m_NearZ = 0.1f;
    float m_FarZ = 1000.0f;

    Matrix4x4 m_ViewMatrix;
    Matrix4x4 m_ProjectionMatrix;
};

} // namespace InvasionEngine 