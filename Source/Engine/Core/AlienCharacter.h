#pragma once
#include "Entity.h"
#include <memory>

namespace InvasionEngine {

class AlienCharacter : public Entity {
public:
    AlienCharacter();
    virtual ~AlienCharacter() = default;

    void TakeDamage(float amount);
    bool IsDead() const { return m_Health <= 0.0f; }
    float GetHealth() const { return m_Health; }

    // Visual feedback
    void TriggerHitFlash();
    bool IsFlashing() const { return m_FlashTimer > 0.0f; }
    void UpdateFlash(float deltaTime);

protected:
    float m_Health;
    float m_FlashTimer;
};

} // namespace InvasionEngine 