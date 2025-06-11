#pragma once

#include <memory>
#include "../Math/Vector.h"
#include "../Math/Matrix.h"

namespace InvasionEngine {

enum class LightType {
    Directional,
    Point,
    Spot
};

class Light {
public:
    Light(LightType type = LightType::Point);
    ~Light();

    // Light properties
    void SetType(LightType type);
    void SetColor(const Vector3& color);
    void SetIntensity(float intensity);
    void SetPosition(const Vector3& position);
    void SetDirection(const Vector3& direction);
    void SetRange(float range);
    void SetSpotAngle(float angle);
    void SetAttenuation(float constant, float linear, float quadratic);

    // Getters
    LightType GetType() const { return m_Type; }
    const Vector3& GetColor() const { return m_Color; }
    float GetIntensity() const { return m_Intensity; }
    const Vector3& GetPosition() const { return m_Position; }
    const Vector3& GetDirection() const { return m_Direction; }
    float GetRange() const { return m_Range; }
    float GetSpotAngle() const { return m_SpotAngle; }
    float GetConstantAttenuation() const { return m_ConstantAttenuation; }
    float GetLinearAttenuation() const { return m_LinearAttenuation; }
    float GetQuadraticAttenuation() const { return m_QuadraticAttenuation; }

    // Transform
    void SetTransform(const Matrix4x4& transform);
    const Matrix4x4& GetTransform() const { return m_Transform; }

private:
    LightType m_Type;
    Vector3 m_Color;
    float m_Intensity;
    Vector3 m_Position;
    Vector3 m_Direction;
    float m_Range;
    float m_SpotAngle;
    float m_ConstantAttenuation;
    float m_LinearAttenuation;
    float m_QuadraticAttenuation;
    Matrix4x4 m_Transform;
};

} // namespace InvasionEngine 