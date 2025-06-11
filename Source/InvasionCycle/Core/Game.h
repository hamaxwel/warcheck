#pragma once

#include "InvasionCycle.h"
#include "Engine/Core/Game.h"
#include "ICGameMode.h"
#include "ICPlayerController.h"
#include <memory>
#include <vector>

namespace InvasionCycle {

class ICPlayerController;
class ICBaseCharacter;
class GameMode;

class Game : public InvasionEngine::Game {
public:
    Game();
    virtual ~Game() = default;

    virtual bool Initialize() override;
    virtual void Update(float deltaTime) override;
    virtual void Render() override;
    virtual void Shutdown() override;

    // Game state management
    void SetGameState(EGameState state);
    EGameState GetGameState() const { return m_GameState; }

    // Player management
    void AddPlayer(std::shared_ptr<ICPlayerController> player);
    void RemovePlayer(std::shared_ptr<ICPlayerController> player);
    const std::vector<std::shared_ptr<ICPlayerController>>& GetPlayers() const { return m_Players; }

    // Game mode
    void SetGameMode(std::shared_ptr<GameMode> gameMode);
    std::shared_ptr<GameMode> GetGameMode() const { return m_GameMode; }

    // Input handling
    virtual void HandleInput() override;

protected:
    EGameState m_GameState;
    std::vector<std::shared_ptr<ICPlayerController>> m_Players;
    std::shared_ptr<GameMode> m_GameMode;
    float m_AccumulatedTime;
    static constexpr float FixedTimeStep = 1.0f / 60.0f; // 60 FPS
    std::shared_ptr<ICGameMode> m_GameMode;
    std::shared_ptr<ICPlayerController> m_PlayerController;
};

} // namespace InvasionCycle 