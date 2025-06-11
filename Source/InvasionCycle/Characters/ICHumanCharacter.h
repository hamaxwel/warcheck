#pragma once

#include "ICBaseCharacter.h"
#include <string>
#include <vector>

class ICHumanCharacter : public ICBaseCharacter {
public:
    ICHumanCharacter();
    virtual ~ICHumanCharacter() override;

    // Human-specific properties
    float Stamina;
    float MaxStamina;
    float StaminaRegenRate;
    float StaminaCostMultiplier;

    // Tactical properties
    float TacticalAwareness;
    float TacticalRange;

    // Combat functions
    void PerformMeleeAttack();
    void PerformRangedAttack();
    void UseTacticalAbility();

protected:
    void UpdateStamina(float DeltaTime);
    void RegenerateStamina(float DeltaTime);
    bool CanUseAbility(float Cost) const;
}; 