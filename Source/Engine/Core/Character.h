#pragma once

#include "Entity.h"
#include <glm/glm.hpp>

namespace InvasionEngine {

class Character : public Entity {
public:
    Character(const std::string& name = "Character");
    virtual ~Character();

    // Core functionality
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual void Shutdown() override;

    // Movement
    void Move(const glm::vec3& direction, float speed);
    void Rotate(const glm::vec3& rotation);
    void Jump();

    // Combat
    virtual void TakeDamage(float amount);
    virtual void Heal(float amount);
    virtual void Die();

    // Properties
    float GetHealth() const { return m_Health; }
    float GetMaxHealth() const { return m_MaxHealth; }
    float GetSpeed() const { return m_Speed; }
    bool IsDead() const { return m_IsDead; }

protected:
    float m_Health;
    float m_MaxHealth;
    float m_Speed;
    bool m_IsDead;
    glm::vec3 m_Velocity;
    glm::vec3 m_Acceleration;
};

} // namespace InvasionEngine 