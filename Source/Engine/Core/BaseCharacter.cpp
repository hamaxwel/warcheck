#include "BaseCharacter.h"

namespace InvasionEngine {

BaseCharacter::BaseCharacter()
    : m_Position(Vector3::Zero)
    , m_Rotation(Quaternion::Identity)
    , m_Velocity(Vector3::Zero)
    , m_Speed(5.0f)
    , m_Health(100.0f)
    , m_MaxHealth(100.0f)
    , m_IsGrounded(false)
{
}

void BaseCharacter::Update(float deltaTime)
{
    // Update position based on velocity
    m_Position += m_Velocity * deltaTime;
}

void BaseCharacter::FixedUpdate(float fixedDeltaTime)
{
    // Apply gravity and handle ground collision
    if (!m_IsGrounded) {
        m_Velocity.y -= 9.81f * fixedDeltaTime; // Apply gravity
    }
}

void BaseCharacter::Move(const Vector3& direction, float speed)
{
    Vector3 moveDirection = direction;
    moveDirection.y = 0.0f; // Keep movement on the ground plane
    moveDirection.Normalize();
    
    m_Velocity = moveDirection * speed;
}

void BaseCharacter::Rotate(const Vector3& axis, float angle)
{
    Quaternion rotation = Quaternion::FromAxisAngle(axis, angle);
    m_Rotation = rotation * m_Rotation;
}

void BaseCharacter::Jump()
{
    if (m_IsGrounded) {
        m_Velocity.y = 5.0f; // Initial jump velocity
        m_IsGrounded = false;
    }
}

void BaseCharacter::TakeDamage(float amount)
{
    m_Health = std::max(0.0f, m_Health - amount);
}

void BaseCharacter::Heal(float amount)
{
    m_Health = std::min(m_MaxHealth, m_Health + amount);
}

} // namespace InvasionEngine 