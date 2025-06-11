#include "WeaponComponent.h"
#include "Entity.h"
#include "AlienCharacter.h"
#include "InputManager.h"
#include "Math/Vector.h"
#include <algorithm>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace InvasionEngine {

WeaponComponent::WeaponComponent(Entity* owner)
    : Component(owner)
    , m_Damage(10.0f)
    , m_Range(100.0f)
    , m_FireRate(1.0f)
    , m_Ammo(30)
    , m_MaxAmmo(30)
    , m_LastFireTime(0.0f)
    , m_ShotLineTimer(0.0f)
{
}

void WeaponComponent::Update(float deltaTime) {
    Entity* entity = GetEntity();
    if (!entity) return;

    InputManager& input = InputManager::GetInstance();

    // Reload mechanic
    if (input.IsKeyPressed(Key::R)) {
        m_Ammo = m_MaxAmmo;
        std::cout << "Reloaded! Ammo: " << m_Ammo << std::endl;
    }

    // Check if left mouse button is pressed and enough time has passed since last shot
    if (input.IsMouseButtonPressed(MouseButton::Left) && m_Ammo > 0 && deltaTime - m_LastFireTime >= m_FireRate) {
        m_LastFireTime = deltaTime;
        m_Ammo--;
        std::cout << "Player fired! Ammo left: " << m_Ammo << std::endl;

        // Perform raycast to detect hits
        Vector3 origin = entity->GetPosition();
        Vector3 direction = entity->GetRotation() * Vector3(0.0f, 0.0f, 1.0f); // Forward direction
        Vector3 target = origin + direction * m_Range;
        m_LastShotOrigin = origin;
        m_LastShotTarget = target;
        m_ShotLineTimer = 0.1f; // Show shot line for 0.1 seconds

        // Simple raycast: check distance to each alien
        for (auto& e : entity->GetScene()->GetEntities()) {
            if (auto* alien = dynamic_cast<AlienCharacter*>(e.get())) {
                Vector3 alienPos = alien->GetPosition();
                Vector3 toAlien = alienPos - origin;
                float dist = toAlien.Length();
                if (dist < m_Range) { // Assume hit if within range
                    alien->TakeDamage(m_Damage);
                    std::cout << "Alien hit! Health: " << alien->GetHealth() << std::endl;
                    if (alien->IsDead()) {
                        std::cout << "Alien killed!" << std::endl;
                    }
                    break; // Hit only one alien per shot
                }
            }
        }
    }

    // Decrement shot line timer
    if (m_ShotLineTimer > 0.0f) {
        m_ShotLineTimer -= deltaTime;
        if (m_ShotLineTimer < 0.0f) m_ShotLineTimer = 0.0f;
    }
}

bool WeaponComponent::Fire(const glm::vec3& direction)
{
    if (m_LastFireTime < 1.0f / m_FireRate)
        return false;

    // Get owner's transform
    const glm::vec3& position = GetOwner()->GetPosition();
    const glm::quat& rotation = GetOwner()->GetRotation();

    // Create ray for hit detection
    glm::vec3 rayStart = position;
    glm::vec3 rayEnd = position + direction * m_Range;

    // TODO: Implement hit detection
    // For now, just reset the fire timer
    m_LastFireTime = 0.0f;
    return true;
}

void WeaponComponent::SetDamage(float damage)
{
    m_Damage = damage;
}

void WeaponComponent::SetRange(float range)
{
    m_Range = range;
}

void WeaponComponent::SetFireRate(float fireRate)
{
    m_FireRate = fireRate;
}

float WeaponComponent::GetDamage() const
{
    return m_Damage;
}

float WeaponComponent::GetRange() const
{
    return m_Range;
}

float WeaponComponent::GetFireRate() const
{
    return m_FireRate;
}

} // namespace InvasionEngine 