#pragma once
#include "Entity.h"
#include <memory>

namespace InvasionEngine {

class HumanCharacter : public Entity {
public:
    HumanCharacter();
    virtual ~HumanCharacter() = default;

    void TakeDamage(float amount);
    bool IsDead() const { return m_Health <= 0.0f; }
    float GetHealth() const { return m_Health; }

protected:
    float m_Health;
};

} // namespace InvasionEngine 