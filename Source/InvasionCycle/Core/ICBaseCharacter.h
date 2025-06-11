#pragma once

#include "Engine/Core/Entity.h"
#include "Engine/Math/Matrix.h"
#include "InvasionCycle.h"
#include <memory>
#include <glm/glm.hpp>

namespace InvasionCycle {

class ICPlayerController;

class ICBaseCharacter : public InvasionEngine::Entity {
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
    void Turn(float angle);
    void LookUp(float angle);
    void Jump();

    // Combat
    void Fire();
    void TakeDamage(float damage);
    void Heal(float amount);
    bool IsAlive() const { return m_Health > 0.0f; }

    // Controller
    void SetController(ICPlayerController* controller) { m_Controller = controller; }
    ICPlayerController* GetController() const { return m_Controller; }

    // Faction
    void SetFaction(EFactionType faction) { m_Faction = faction; }
    EFactionType GetFaction() const { return m_Faction; }

    // Health and movement properties
    float GetHealth() const { return m_Health; }
    float GetMaxHealth() const { return m_MaxHealth; }
    float GetMoveSpeed() const { return m_MoveSpeed; }
    float GetJumpForce() const { return m_JumpForce; }

    // Health and movement setters
    void SetHealth(float health) { m_Health = health; }
    void SetMaxHealth(float maxHealth) { m_MaxHealth = maxHealth; }
    void SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }
    void SetJumpForce(float jumpForce) { m_JumpForce = jumpForce; }

protected:
    ICPlayerController* m_Controller;
    EFactionType m_Faction;
    float m_Health;
    float m_MaxHealth;
    float m_MoveSpeed;
    float m_JumpForce;
    bool m_IsJumping;
    glm::vec3 m_Velocity;
    float m_Gravity;
    bool m_IsGrounded;
};

} // namespace InvasionCycle 