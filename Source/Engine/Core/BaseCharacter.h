#pragma once

#include "Component.h"
#include "Math/Vector.h"
#include "Math/Quaternion.h"
#include <memory>

namespace InvasionEngine {

class BaseCharacter : public Component {
public:
    BaseCharacter();
    virtual ~BaseCharacter() = default;

    virtual void Update(float deltaTime) override;
    virtual void FixedUpdate(float fixedDeltaTime) override;

    // Movement
    void Move(const Vector3& direction, float speed);
    void Rotate(const Vector3& axis, float angle);
    void Jump();

    // State
    bool IsGrounded() const { return m_IsGrounded; }
    float GetHealth() const { return m_Health; }
    void TakeDamage(float amount);
    void Heal(float amount);

    // Getters/Setters
    const Vector3& GetPosition() const { return m_Position; }
    void SetPosition(const Vector3& position) { m_Position = position; }
    const Quaternion& GetRotation() const { return m_Rotation; }
    void SetRotation(const Quaternion& rotation) { m_Rotation = rotation; }
    float GetSpeed() const { return m_Speed; }
    void SetSpeed(float speed) { m_Speed = speed; }

protected:
    Vector3 m_Position;
    Quaternion m_Rotation;
    Vector3 m_Velocity;
    float m_Speed;
    float m_Health;
    float m_MaxHealth;
    bool m_IsGrounded;
};

} // namespace InvasionEngine 