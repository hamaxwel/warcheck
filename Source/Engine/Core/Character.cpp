#include "Character.h"

namespace InvasionEngine {

Character::Character(const std::string& name)
    : Entity(name)
    , m_Health(100.0f)
    , m_MaxHealth(100.0f)
    , m_Speed(5.0f)
    , m_IsDead(false)
    , m_Velocity(0.0f)
    , m_Acceleration(0.0f)
{
}

Character::~Character() {
    Shutdown();
}

void Character::Initialize() {
    Entity::Initialize();
}

void Character::Update(float deltaTime) {
    if (m_IsDead) return;

    // Update position based on velocity
    m_Position += m_Velocity * deltaTime;
    
    // Apply acceleration to velocity
    m_Velocity += m_Acceleration * deltaTime;
    
    // Apply damping
    m_Velocity *= 0.95f;
    
    // Reset acceleration
    m_Acceleration = glm::vec3(0.0f);

    Entity::Update(deltaTime);
}

void Character::Render() {
    if (m_IsDead) return;
    Entity::Render();
}

void Character::Shutdown() {
    Entity::Shutdown();
}

void Character::Move(const glm::vec3& direction, float speed) {
    if (m_IsDead) return;
    m_Acceleration += glm::normalize(direction) * speed;
}

void Character::Rotate(const glm::vec3& rotation) {
    if (m_IsDead) return;
    m_Rotation += rotation;
}

void Character::Jump() {
    if (m_IsDead) return;
    // TODO: Implement jumping mechanics
}

void Character::TakeDamage(float amount) {
    if (m_IsDead) return;
    
    m_Health -= amount;
    if (m_Health <= 0.0f) {
        Die();
    }
}

void Character::Heal(float amount) {
    if (m_IsDead) return;
    
    m_Health = std::min(m_Health + amount, m_MaxHealth);
}

void Character::Die() {
    if (m_IsDead) return;
    
    m_IsDead = true;
    m_Health = 0.0f;
    // TODO: Trigger death animation and cleanup
}

} // namespace InvasionEngine 
 