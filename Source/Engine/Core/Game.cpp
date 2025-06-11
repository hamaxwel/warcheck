#include "Game.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "UIManager.h"
#include <chrono>

namespace InvasionEngine {

Game::Game()
    : m_AccumulatedTime(0.0f)
{
}

bool Game::Initialize()
{
    m_GameMode.reset();
    m_AccumulatedTime = 0.0f;
    return true;
}

void Game::Update(float deltaTime)
{
    // Handle input
    HandleInput();

    // Fixed time step update
    m_AccumulatedTime += deltaTime;
    while (m_AccumulatedTime >= FixedTimeStep) {
        if (m_GameMode) {
            m_GameMode->Update(FixedTimeStep);
        }
        m_AccumulatedTime -= FixedTimeStep;
    }
}

void Game::Render()
{
    if (m_GameMode) {
        m_GameMode->Render();
    }
}

void Game::Shutdown()
{
    m_GameMode.reset();
}

void Game::SetGameMode(std::shared_ptr<GameMode> gameMode)
{
    m_GameMode = gameMode;
}

void Game::HandleInput()
{
    // Base class implementation does nothing
    // Derived classes should override this method
}

bool GameApplication::Initialize(std::shared_ptr<Game> game) {
    m_Game = game;

    // Initialize the engine
    if (!m_Engine.Initialize()) {
        return false;
    }

    // Initialize the game
    if (!m_Game->Initialize()) {
        return false;
    }

    return true;
}

void GameApplication::Shutdown() {
    if (m_Game) {
        m_Game->Shutdown();
    }
    m_Engine.Shutdown();
}

void GameApplication::Run() {
    auto lastTime = std::chrono::high_resolution_clock::now();
    float accumulator = 0.0f;
    const float fixedTimeStep = 1.0f / 60.0f;

    while (m_Game->IsRunning()) {
        // Calculate delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Update game state
        m_Game->Update(deltaTime);

        // Fixed time step update
        accumulator += deltaTime;
        while (accumulator >= fixedTimeStep) {
            // Update physics, etc.
            accumulator -= fixedTimeStep;
        }

        // Render frame
        m_Game->Render();

        // Process window events
        m_Engine.GetWindow()->PollEvents();
    }
}

void Game::Update(float deltaTime) {
    // ... existing update logic ...
    GetSceneManager().Update(deltaTime);

    // Check for win/lose and restart
    const std::string& msg = UIManager::GetInstance().GetMessage();
    if (!msg.empty()) {
        InputManager& input = InputManager::GetInstance();
        if (input.IsKeyPressed(Key::Enter)) {
            GetSceneManager().Reset();
        }
    }
}

} // namespace InvasionEngine 