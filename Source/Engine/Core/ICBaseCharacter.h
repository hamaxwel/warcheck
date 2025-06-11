#pragma once

#include "Entity.h"
#include <glm/glm.hpp>

namespace InvasionEngine {

class ICBaseCharacter : public Entity {
public:
    ICBaseCharacter();
    virtual ~ICBaseCharacter() = default;

    virtual bool Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual void Shutdown() override;

    // Movement
    void Move(const glm::vec3& direction, float deltaTime);
    void Rotate(const glm::vec3& rotation, float deltaTime);

    // Health
    float GetHealth() const { return m_Health; }
    void SetHealth(float health) { m_Health = health; }
    void TakeDamage(float damage);
    bool IsDead() const { return m_Health <= 0.0f; }

    // Speed
    float GetMovementSpeed() const { return m_MovementSpeed; }
    void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }

protected:
    float m_Health;
    float m_MovementSpeed;
};

} // namespace InvasionEngine 