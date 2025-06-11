#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Entity.h"
#include "PlayerController.h"

namespace InvasionEngine {

class Entity;
class PlayerController;

class GameMode {
public:
    GameMode();
    virtual ~GameMode() = default;

    // Core functionality
    virtual bool Initialize();
    virtual void Update(float deltaTime);
    virtual void Render();
    virtual void Shutdown();

    // Game state management
    virtual void StartGame();
    virtual void EndGame();
    virtual void PauseGame();
    virtual void ResumeGame();
    virtual bool IsGameActive() const { return m_IsGameActive; }
    virtual bool IsPaused() const { return m_IsPaused; }

    // Entity management
    void AddEntity(std::shared_ptr<Entity> entity);
    void RemoveEntity(std::shared_ptr<Entity> entity);
    const std::vector<std::shared_ptr<Entity>>& GetEntities() const { return m_Entities; }

    // Player management
    void AddPlayer(std::shared_ptr<PlayerController> player);
    void RemovePlayer(std::shared_ptr<PlayerController> player);
    const std::vector<std::shared_ptr<PlayerController>>& GetPlayers() const { return m_Players; }

    // Game rules
    virtual void SetGameTime(float time) { m_GameTime = time; }
    virtual float GetGameTime() const { return m_GameTime; }
    virtual void SetMaxPlayers(int count) { m_MaxPlayers = count; }
    virtual int GetMaxPlayers() const { return m_MaxPlayers; }

    virtual void SpawnPlayer();
    virtual void SpawnEnemy();
    virtual void SpawnPickup();

    void SetPlayerController(std::shared_ptr<PlayerController> controller);
    std::shared_ptr<PlayerController> GetPlayerController() const { return m_PlayerController; }

protected:
    bool m_IsGameActive;
    bool m_IsPaused;
    float m_GameTime;
    int m_MaxPlayers;
    std::vector<std::shared_ptr<Entity>> m_Entities;
    std::vector<std::shared_ptr<PlayerController>> m_Players;
    std::shared_ptr<PlayerController> m_PlayerController;
};

} // namespace InvasionEngine 