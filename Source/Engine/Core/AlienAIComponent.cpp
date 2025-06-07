#include "AlienAIComponent.h"
#include "Entity.h"
#include "HumanCharacter.h"
#include "Math/Vector.h"
#include <algorithm>
#include <iostream>

namespace InvasionEngine {

AlienAIComponent::AlienAIComponent() : m_State(State::Patrol), m_CurrentPatrolIndex(0), m_ChaseSpeed(3.0f), m_AttackRange(2.0f), m_AttackDamage(10.0f), m_AttackCooldown(1.0f), m_AttackTimer(0.0f) {}

void AlienAIComponent::Update(float deltaTime) {
    Entity* entity = GetEntity();
    if (!entity) return;

    // Find the player (human)
    HumanCharacter* player = nullptr;
    for (auto& e : entity->GetScene()->GetEntities()) {
        if (auto* human = dynamic_cast<HumanCharacter*>(e.get())) {
            player = human;
            break;
        }
    }

    if (!player) return;

    Vector3 alienPos = entity->GetPosition();
    Vector3 playerPos = player->GetPosition();
    float distToPlayer = (playerPos - alienPos).Length();

    // State machine
    switch (m_State) {
        case State::Patrol:
            if (distToPlayer < 5.0f) {
                m_State = State::Chase;
            } else if (!m_PatrolPoints.empty()) {
                Vector3 target = m_PatrolPoints[m_CurrentPatrolIndex];
                Vector3 dir = (target - alienPos).Normalized();
                alienPos += dir * m_ChaseSpeed * deltaTime;
                entity->SetPosition(alienPos);

                if ((target - alienPos).LengthSquared() < 0.1f) {
                    m_CurrentPatrolIndex = (m_CurrentPatrolIndex + 1) % m_PatrolPoints.size();
                }
            }
            break;

        case State::Chase:
            if (distToPlayer < m_AttackRange) {
                m_State = State::Attack;
            } else if (distToPlayer > 10.0f) {
                m_State = State::Patrol;
            } else {
                Vector3 dir = (playerPos - alienPos).Normalized();
                alienPos += dir * m_ChaseSpeed * deltaTime;
                entity->SetPosition(alienPos);
            }
            break;

        case State::Attack:
            if (distToPlayer > m_AttackRange) {
                m_State = State::Chase;
            } else {
                m_AttackTimer -= deltaTime;
                if (m_AttackTimer <= 0.0f) {
                    player->TakeDamage(m_AttackDamage);
                    std::cout << "Player hit by alien! Player health: " << player->GetHealth() << std::endl;
                    m_AttackTimer = m_AttackCooldown;
                }
            }
            break;
    }
}

void AlienAIComponent::SetPatrolPoints(const std::vector<Vector3>& points) {
    m_PatrolPoints = points;
    m_CurrentPatrolIndex = 0;
}

} // namespace InvasionEngine 