#include "GameMode.h"
#include "Entity.h"
#include "PlayerController.h"
#include <algorithm>

namespace InvasionEngine {

GameMode::GameMode()
    : m_IsGameActive(false)
    , m_IsPaused(false)
    , m_GameTime(0.0f)
    , m_MaxPlayers(4)
{
}

bool GameMode::Initialize()
{
    m_Entities.clear();
    m_Players.clear();
    m_IsGameActive = false;
    m_IsPaused = false;
    m_GameTime = 0.0f;
    return true;
}

void GameMode::Update(float deltaTime)
{
    if (!m_IsGameActive || m_IsPaused) return;

    m_GameTime += deltaTime;

    // Update all entities
    for (auto& entity : m_Entities) {
        if (entity) {
            entity->Update(deltaTime);
        }
    }

    // Update all players
    for (auto& player : m_Players) {
        if (player) {
            player->Update(deltaTime);
        }
    }
}

void GameMode::Render()
{
    if (!m_IsGameActive) return;

    // Render all entities
    for (auto& entity : m_Entities) {
        if (entity) {
            entity->Render();
        }
    }

    // Render all players
    for (auto& player : m_Players) {
        if (player) {
            player->Render();
        }
    }
}

void GameMode::Shutdown()
{
    m_Entities.clear();
    m_Players.clear();
    m_IsGameActive = false;
    m_IsPaused = false;
    m_GameTime = 0.0f;
}

void GameMode::StartGame() {
    if (m_IsGameActive) return;
    
    m_IsGameActive = true;
    m_IsPaused = false;
    m_GameTime = 0.0f;
}

void GameMode::EndGame() {
    if (!m_IsGameActive) return;
    
    m_IsGameActive = false;
    m_IsPaused = false;
    m_GameTime = 0.0f;
}

void GameMode::PauseGame() {
    if (!m_IsGameActive || m_IsPaused) return;
    m_IsPaused = true;
}

void GameMode::ResumeGame() {
    if (!m_IsGameActive || !m_IsPaused) return;
    m_IsPaused = false;
}

void GameMode::AddEntity(std::shared_ptr<Entity> entity)
{
    m_Entities.push_back(entity);
}

void GameMode::RemoveEntity(std::shared_ptr<Entity> entity)
{
    auto it = std::find(m_Entities.begin(), m_Entities.end(), entity);
    if (it != m_Entities.end()) {
        m_Entities.erase(it);
    }
}

void GameMode::AddPlayer(std::shared_ptr<PlayerController> player)
{
    m_Players.push_back(player);
}

void GameMode::RemovePlayer(std::shared_ptr<PlayerController> player)
{
    auto it = std::find(m_Players.begin(), m_Players.end(), player);
    if (it != m_Players.end()) {
        m_Players.erase(it);
    }
}

void GameMode::SpawnPlayer()
{
    // TODO: Implement player spawning
}

void GameMode::SpawnEnemy()
{
    // TODO: Implement enemy spawning
}

void GameMode::SpawnPickup()
{
    // TODO: Implement pickup spawning
}

void GameMode::SetPlayerController(std::shared_ptr<PlayerController> controller)
{
    m_PlayerController = controller;
}

} // namespace InvasionEngine 