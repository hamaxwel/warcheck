#include "Game.h"
#include "ICPlayerController.h"
#include "ICBaseCharacter.h"
#include "ICGameMode.h"
#include "Engine/Core/InputManager.h"
#include <algorithm>
#include <GLFW/glfw3.h>

namespace InvasionCycle {

Game::Game()
    : m_GameState(EGameState::None)
    , m_AccumulatedTime(0.0f)
    , m_GameMode(std::make_shared<ICGameMode>())
    , m_PlayerController(std::make_shared<ICPlayerController>())
{
}

bool Game::Initialize()
{
    m_GameState = EGameState::MainMenu;
    m_Players.clear();
    m_GameMode->Initialize();
    m_PlayerController->Initialize();
    m_AccumulatedTime = 0.0f;
    return true;
}

void Game::Update(float deltaTime)
{
    // Handle input
    HandleInput();

    // Update game state
    switch (m_GameState) {
        case EGameState::MainMenu:
            // TODO: Update main menu
            break;

        case EGameState::Loading:
            // TODO: Handle loading
            break;

        case EGameState::Playing:
            // Fixed time step update
            m_AccumulatedTime += deltaTime;
            while (m_AccumulatedTime >= FixedTimeStep) {
                if (m_GameMode) {
                    m_GameMode->Update(FixedTimeStep);
                }
                m_AccumulatedTime -= FixedTimeStep;
            }

            // Update all players
            for (auto& player : m_Players) {
                if (player) {
                    player->Update(deltaTime);
                }
            }
            break;

        case EGameState::Paused:
            // TODO: Handle pause menu
            break;

        case EGameState::GameOver:
            // TODO: Handle game over state
            break;

        default:
            break;
    }
}

void Game::Render()
{
    switch (m_GameState) {
        case EGameState::MainMenu:
            // TODO: Render main menu
            break;

        case EGameState::Loading:
            // TODO: Render loading screen
            break;

        case EGameState::Playing:
            if (m_GameMode) {
                m_GameMode->Render();
            }
            // Render all players
            for (auto& player : m_Players) {
                if (player) {
                    player->Render();
                }
            }
            break;

        case EGameState::Paused:
            // TODO: Render pause menu
            break;

        case EGameState::GameOver:
            // TODO: Render game over screen
            break;

        default:
            break;
    }
}

void Game::Shutdown()
{
    m_Players.clear();
    m_GameMode->Shutdown();
    m_PlayerController->Shutdown();
}

void Game::SetGameState(EGameState state)
{
    m_GameState = state;
}

void Game::AddPlayer(std::shared_ptr<ICPlayerController> player)
{
    if (player) {
        m_Players.push_back(player);
    }
}

void Game::RemovePlayer(std::shared_ptr<ICPlayerController> player)
{
    auto it = std::find(m_Players.begin(), m_Players.end(), player);
    if (it != m_Players.end()) {
        m_Players.erase(it);
    }
}

void Game::SetGameMode(std::shared_ptr<GameMode> gameMode)
{
    m_GameMode = gameMode;
}

void Game::HandleInput()
{
    // Handle input for the player controller
    if (m_PlayerController) {
        InvasionEngine::InputState state;
        state.forward = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS;
        state.backward = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS;
        state.left = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS;
        state.right = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS;
        state.jump = glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS;

        double mouseX, mouseY;
        glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);
        state.mouseX = static_cast<float>(mouseX);
        state.mouseY = static_cast<float>(mouseY);

        state.mouseLeft = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        state.mouseRight = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

        m_PlayerController->SetInputState(state);
    }
}

} // namespace InvasionCycle 