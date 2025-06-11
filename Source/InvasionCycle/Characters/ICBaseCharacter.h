#pragma once

#include "Tactical/ICTacticalCommandInterface.h"
#include "InvasionCycle.h"
#include "BaseCharacter.h"
#include <string>

class ICBaseCharacter : public BaseCharacter, public ITacticalCommandInterface {
public:
    ICBaseCharacter();
    virtual ~ICBaseCharacter();
    void BeginPlay();
    void Tick(float deltaTime);
    void SetupPlayerInputComponent(void* inputComponent); // void* as a stub for UInputComponent

    // Command interface
    void ExecuteCommand(const std::string& commandData);
    void CancelCommand();
    bool CanExecuteCommand(const std::string& commandData) const;
    void JoinSquad(void* squadLeader);
    void LeaveSquad();
    std::string GetStatusReport() const;
    bool IsCommandComplete() const;

    // Health and damage
    void TakeDamage(float damageAmount, void* damageEvent, void* eventInstigator, void* damageCauser);
    void OnHealthChanged();

    // Game state
    float Health;
    float MaxHealth;
    bool bIsAlive;
    // Add other members as needed
}; 