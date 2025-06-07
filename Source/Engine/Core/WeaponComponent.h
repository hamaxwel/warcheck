#pragma once
#include "Component.h"
#include "Math/Vector.h"
#include <memory>

namespace InvasionEngine {

class WeaponComponent : public Component {
public:
    WeaponComponent();
    virtual ~WeaponComponent() = default;

    virtual void Update(float deltaTime) override;

    void SetDamage(float damage) { m_Damage = damage; }
    void SetFireRate(float rate) { m_FireRate = rate; }
    void SetAmmo(int ammo) { m_Ammo = ammo; m_MaxAmmo = ammo; }
    int GetAmmo() const { return m_Ammo; }

    // Visual feedback
    bool IsShotLineActive() const { return m_ShotLineTimer > 0.0f; }
    Vector3 GetShotOrigin() const { return m_LastShotOrigin; }
    Vector3 GetShotTarget() const { return m_LastShotTarget; }

private:
    float m_Damage;
    float m_FireRate;
    int m_Ammo;
    int m_MaxAmmo;
    float m_LastFireTime;

    // Visual feedback
    float m_ShotLineTimer;
    Vector3 m_LastShotOrigin;
    Vector3 m_LastShotTarget;
};

} // namespace InvasionEngine 