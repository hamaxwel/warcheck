#pragma once

#include "Engine/Core/PlayerController.h"
#include "ICBaseCharacter.h"
#include <memory>

namespace InvasionCycle {

class ICPlayerController : public InvasionEngine::PlayerController {
public:
    ICPlayerController();
    virtual ~ICPlayerController() = default;

    virtual bool Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Shutdown() override;

    void SetCharacter(std::shared_ptr<ICBaseCharacter> character);
    std::shared_ptr<ICBaseCharacter> GetCharacter() const { return m_Character; }

    // Faction management
    void SetFaction(EFactionType faction) { m_Faction = faction; }
    EFactionType GetFaction() const { return m_Faction; }

    // Input handling
    virtual void HandleInput() override;
    void MoveForward(float value);
    void MoveRight(float value);
    void Turn(float value);
    void LookUp(float value);
    void Jump();
    void Fire();

protected:
    std::shared_ptr<ICBaseCharacter> m_Character;
    EFactionType m_Faction;
    glm::vec3 m_MovementInput;
    float m_TurnInput;
    float m_LookUpInput;
    bool m_IsJumping;
    bool m_IsFiring;
};

} // namespace InvasionCycle 