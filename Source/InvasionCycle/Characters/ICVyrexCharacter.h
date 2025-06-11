#pragma once
#include "BaseCharacter.h"
#include <string>

class ICVyrexCharacter : public BaseCharacter {
public:
    ICVyrexCharacter();
    ~ICVyrexCharacter();
    void BeginPlay();
    void Tick(float deltaTime);
    void SetupPlayerInputComponent(void* inputComponent); // void* as a stub
    void TakeDamage(float damageAmount, void* damageEvent, void* eventInstigator, void* damageCauser);
    void OnHealthChanged();
    void BeginDestroy();
    // Vyrex-specific properties
    float BiomassRegenerationRate;
    float CurrentBiomass;
    float MaxBiomass;
    // Evolution traits
    float CarapaceArmor;
    float AcidDamage;
    // Special abilities
    float BurrowSpeed;
    float BurrowCooldown;
    float SpitRange;
    float SpitCooldown;
    // Combat abilities
    void Burrow();
    void Unburrow();
    void SpitAcid(const FVector& TargetLocation);
    // Add any other members as needed
}; 