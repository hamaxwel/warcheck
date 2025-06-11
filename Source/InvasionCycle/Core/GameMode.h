#pragma once

#include "InvasionCycle.h"
#include <Engine/Core/GameMode.h>
#include <map>

namespace InvasionCycle {

class GameMode : public InvasionEngine::GameMode {
public:
    GameMode();
    virtual ~GameMode();

    // Core functionality
    virtual void Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual void Shutdown() override;

    // Game state management
    virtual void StartGame() override;
    virtual void EndGame() override;
    virtual void PauseGame() override;
    virtual void ResumeGame() override;

    // Player management
    virtual void AddPlayer(std::shared_ptr<InvasionEngine::Entity> player) override;
    virtual void RemovePlayer(std::shared_ptr<InvasionEngine::Entity> player) override;

    // Faction management
    void SetPlayerFaction(std::shared_ptr<InvasionEngine::Entity> player, Constants::FactionType faction);
    Constants::FactionType GetPlayerFaction(std::shared_ptr<InvasionEngine::Entity> player) const;
    int GetFactionScore(Constants::FactionType faction) const;
    void AddFactionScore(Constants::FactionType faction, int points);

protected:
    std::map<std::shared_ptr<InvasionEngine::Entity>, Constants::FactionType> m_PlayerFactions;
    std::map<Constants::FactionType, int> m_FactionScores;
};

} // namespace InvasionCycle 