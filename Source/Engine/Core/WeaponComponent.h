#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace InvasionEngine {

class Entity;

class WeaponComponent : public Component {
public:
    WeaponComponent(Entity* owner);
    virtual ~WeaponComponent() = default;

    virtual void Update(float deltaTime) override;

    bool Fire(const glm::vec3& direction);

    // Getters and setters
    void SetDamage(float damage);
    void SetRange(float range);
    void SetFireRate(float fireRate);
    float GetDamage() const;
    float GetRange() const;
    float GetFireRate() const;

    // Visual feedback
    bool IsShotLineActive() const { return m_ShotLineTimer > 0.0f; }
    glm::vec3 GetShotOrigin() const { return m_LastShotOrigin; }
    glm::vec3 GetShotTarget() const { return m_LastShotTarget; }

private:
    float m_Damage;
    float m_Range;
    float m_FireRate;
    float m_LastFireTime;
    int m_Ammo;
    int m_MaxAmmo;
    float m_ShotLineTimer;
    glm::vec3 m_LastShotOrigin;
    glm::vec3 m_LastShotTarget;
};

} // namespace InvasionEngine 