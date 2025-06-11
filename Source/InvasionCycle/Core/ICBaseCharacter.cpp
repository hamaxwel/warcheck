#include "ICBaseCharacter.h"
#include "ICPlayerController.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace InvasionCycle {

ICBaseCharacter::ICBaseCharacter()
    : m_Controller(nullptr)
    , m_Faction(EFactionType::None)
    , m_Health(100.0f)
    , m_MaxHealth(100.0f)
    , m_MoveSpeed(5.0f)
    , m_JumpForce(10.0f)
    , m_IsJumping(false)
    , m_Velocity(0.0f)
    , m_Gravity(-9.81f)
{
}

ICBaseCharacter::~ICBaseCharacter()
{
}

bool ICBaseCharacter::Initialize()
{
    if (!Entity::Initialize()) {
        return false;
    }
    m_Controller = nullptr;
    m_Faction = EFactionType::None;
    m_Health = m_MaxHealth;
    m_Velocity = glm::vec3(0.0f);
    m_IsJumping = false;
    return true;
}

void ICBaseCharacter::Update(float deltaTime)
{
    Entity::Update(deltaTime);

    // Update velocity based on gravity
    if (m_IsJumping) {
        m_Velocity.y -= m_Gravity * deltaTime;
    }

    // Update position based on velocity
    glm::vec3 position = GetPosition();
    position += m_Velocity * deltaTime;
    SetPosition(position);

    // Check if landed
    if (position.y <= 0.0f) {
        position.y = 0.0f;
        SetPosition(position);
        m_Velocity.y = 0.0f;
        m_IsJumping = false;
    }
}

void ICBaseCharacter::Render()
{
    Entity::Render();
}

void ICBaseCharacter::Shutdown()
{
    Entity::Shutdown();
    m_Controller = nullptr;
}

void ICBaseCharacter::Move(const glm::vec3& direction, float deltaTime)
{
    glm::vec3 movement = direction * m_MoveSpeed * deltaTime;
    SetPosition(GetPosition() + movement);
}

void ICBaseCharacter::Rotate(const glm::vec3& rotation, float deltaTime)
{
    glm::quat rot = glm::quat(rotation * deltaTime);
    SetRotation(GetRotation() * rot);
}

void ICBaseCharacter::Jump()
{
    if (!m_IsJumping) {
        m_Velocity.y = m_JumpForce;
        m_IsJumping = true;
    }
}

void ICBaseCharacter::Fire()
{
    // TODO: Implement weapon firing
    // This would involve creating projectiles or performing raycasts
}

void ICBaseCharacter::SetHealth(float health)
{
    m_Health = glm::clamp(health, 0.0f, m_MaxHealth);
}

void ICBaseCharacter::SetMaxHealth(float maxHealth)
{
    m_MaxHealth = glm::max(0.0f, maxHealth);
    m_Health = glm::min(m_Health, m_MaxHealth);
}

void ICBaseCharacter::SetMoveSpeed(float moveSpeed)
{
    m_MoveSpeed = glm::max(0.0f, moveSpeed);
}

void ICBaseCharacter::SetJumpForce(float jumpForce)
{
    m_JumpForce = glm::max(0.0f, jumpForce);
}

void ICBaseCharacter::TakeDamage(float damage)
{
    SetHealth(m_Health - damage);
}

void ICBaseCharacter::Heal(float amount)
{
    SetHealth(m_Health + amount);
}

bool ICBaseCharacter::IsAlive() const
{
    return m_Health > 0.0f;
}

} // namespace InvasionCycle 