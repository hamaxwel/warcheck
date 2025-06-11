#include "GameMode.h"

namespace InvasionCycle {

GameMode::GameMode()
{
}

GameMode::~GameMode() {
    Shutdown();
}

void GameMode::Initialize() {
    InvasionEngine::GameMode::Initialize();
    
    // Initialize faction scores
    m_FactionScores[Constants::FactionType::Human] = 0;
    m_FactionScores[Constants::FactionType::Vyrex] = 0;
}

void GameMode::Update(float deltaTime) {
    InvasionEngine::GameMode::Update(deltaTime);
}

void GameMode::Render() {
    InvasionEngine::GameMode::Render();
}

void GameMode::Shutdown() {
    m_PlayerFactions.clear();
    m_FactionScores.clear();
    InvasionEngine::GameMode::Shutdown();
}

void GameMode::StartGame() {
    InvasionEngine::GameMode::StartGame();
}

void GameMode::EndGame() {
    InvasionEngine::GameMode::EndGame();
}

void GameMode::PauseGame() {
    InvasionEngine::GameMode::PauseGame();
}

void GameMode::ResumeGame() {
    InvasionEngine::GameMode::ResumeGame();
}

void GameMode::AddPlayer(std::shared_ptr<InvasionEngine::Entity> player) {
    InvasionEngine::GameMode::AddPlayer(player);
    // Default faction assignment
    m_PlayerFactions[player] = Constants::FactionType::Human;
}

void GameMode::RemovePlayer(std::shared_ptr<InvasionEngine::Entity> player) {
    m_PlayerFactions.erase(player);
    InvasionEngine::GameMode::RemovePlayer(player);
}

void GameMode::SetPlayerFaction(std::shared_ptr<InvasionEngine::Entity> player, Constants::FactionType faction) {
    if (m_PlayerFactions.find(player) != m_PlayerFactions.end()) {
        m_PlayerFactions[player] = faction;
    }
}

Constants::FactionType GameMode::GetPlayerFaction(std::shared_ptr<InvasionEngine::Entity> player) const {
    auto it = m_PlayerFactions.find(player);
    if (it != m_PlayerFactions.end()) {
        return it->second;
    }
    return Constants::FactionType::Human; // Default faction
}

int GameMode::GetFactionScore(Constants::FactionType faction) const {
    auto it = m_FactionScores.find(faction);
    if (it != m_FactionScores.end()) {
        return it->second;
    }
    return 0;
}

void GameMode::AddFactionScore(Constants::FactionType faction, int points) {
    m_FactionScores[faction] += points;
}

} // namespace InvasionCycle 