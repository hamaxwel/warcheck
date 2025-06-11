#pragma once

#include "Component.h"
#include <memory>
#include <vector>

namespace InvasionEngine {

class BaseGameMode : public Component {
public:
    BaseGameMode();
    virtual ~BaseGameMode() = default;

    virtual void Update(float deltaTime) override;
    virtual void FixedUpdate(float fixedDeltaTime) override;

    // Game state
    virtual void StartGame();
    virtual void EndGame();
    virtual void PauseGame();
    virtual void ResumeGame();

    // Game flow
    virtual void OnGameStart();
    virtual void OnGameEnd();
    virtual void OnGamePause();
    virtual void OnGameResume();

    // Getters/Setters
    bool IsGameRunning() const { return m_IsGameRunning; }
    bool IsGamePaused() const { return m_IsGamePaused; }
    float GetGameTime() const { return m_GameTime; }

protected:
    bool m_IsGameRunning;
    bool m_IsGamePaused;
    float m_GameTime;
};

} // namespace InvasionEngine 