#pragma once
#include "Component.h"
#include "Math/Vector.h"
#include <vector>

namespace InvasionEngine {

class AlienAIComponent : public Component {
public:
    AlienAIComponent();
    virtual ~AlienAIComponent() = default;

    virtual void Update(float deltaTime) override;

    void SetPatrolPoints(const std::vector<Vector3>& points);
    void SetChaseSpeed(float speed) { m_ChaseSpeed = speed; }
    void SetAttackRange(float range) { m_AttackRange = range; }
    void SetAttackDamage(float damage) { m_AttackDamage = damage; }
    void SetAttackCooldown(float cooldown) { m_AttackCooldown = cooldown; }

private:
    enum class State {
        Patrol,
        Chase,
        Attack
    };

    State m_State;
    std::vector<Vector3> m_PatrolPoints;
    size_t m_CurrentPatrolIndex;
    float m_ChaseSpeed;
    float m_AttackRange;
    float m_AttackDamage;
    float m_AttackCooldown;
    float m_AttackTimer;
};

} // namespace InvasionEngine 