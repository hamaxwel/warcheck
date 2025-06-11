#include "BaseGameMode.h"

namespace InvasionEngine {

BaseGameMode::BaseGameMode()
    : m_IsGameRunning(false)
    , m_IsGamePaused(false)
    , m_GameTime(0.0f)
{
}

void BaseGameMode::Update(float deltaTime)
{
    if (m_IsGameRunning && !m_IsGamePaused) {
        m_GameTime += deltaTime;
    }
}

void BaseGameMode::FixedUpdate(float fixedDeltaTime)
{
    // Handle fixed update logic here
}

void BaseGameMode::StartGame()
{
    if (!m_IsGameRunning) {
        m_IsGameRunning = true;
        m_IsGamePaused = false;
        m_GameTime = 0.0f;
        OnGameStart();
    }
}

void BaseGameMode::EndGame()
{
    if (m_IsGameRunning) {
        m_IsGameRunning = false;
        m_IsGamePaused = false;
        OnGameEnd();
    }
}

void BaseGameMode::PauseGame()
{
    if (m_IsGameRunning && !m_IsGamePaused) {
        m_IsGamePaused = true;
        OnGamePause();
    }
}

void BaseGameMode::ResumeGame()
{
    if (m_IsGameRunning && m_IsGamePaused) {
        m_IsGamePaused = false;
        OnGameResume();
    }
}

void BaseGameMode::OnGameStart()
{
    // Override in derived classes
}

void BaseGameMode::OnGameEnd()
{
    // Override in derived classes
}

void BaseGameMode::OnGamePause()
{
    // Override in derived classes
}

void BaseGameMode::OnGameResume()
{
    // Override in derived classes
}

} // namespace InvasionEngine 