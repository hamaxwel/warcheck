#include "Light.h"

namespace InvasionEngine {

Light::Light(LightType type) : m_Type(type), m_Intensity(1.0f), m_Range(10.0f), m_SpotAngle(45.0f) {
    m_Color = Vector3(1.0f, 1.0f, 1.0f);
    m_Position = Vector3(0.0f, 0.0f, 0.0f);
    m_Direction = Vector3(0.0f, -1.0f, 0.0f);
    m_ConstantAttenuation = 1.0f;
    m_LinearAttenuation = 0.09f;
    m_QuadraticAttenuation = 0.032f;
    m_Transform = Matrix4x4::Identity();
}

Light::~Light() {}

void Light::SetType(LightType type) {
    m_Type = type;
}

void Light::SetColor(const Vector3& color) {
    m_Color = color;
}

void Light::SetIntensity(float intensity) {
    m_Intensity = intensity;
}

void Light::SetPosition(const Vector3& position) {
    m_Position = position;
}

void Light::SetDirection(const Vector3& direction) {
    m_Direction = direction;
}

void Light::SetRange(float range) {
    m_Range = range;
}

void Light::SetSpotAngle(float angle) {
    m_SpotAngle = angle;
}

void Light::SetAttenuation(float constant, float linear, float quadratic) {
    m_ConstantAttenuation = constant;
    m_LinearAttenuation = linear;
    m_QuadraticAttenuation = quadratic;
}

void Light::SetTransform(const Matrix4x4& transform) {
    m_Transform = transform;
}

} // namespace InvasionEngine 