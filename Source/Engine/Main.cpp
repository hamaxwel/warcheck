#include "Engine/Core/GameApplication.h"
#include "InvasionCycle/Core/Game.h"
#include <memory>

int main(int argc, char* argv[])
{
    // Create game application
    auto app = std::make_unique<InvasionEngine::GameApplication>();
    if (!app->Initialize("Invasion Cycle", 1280, 720)) {
        return 1;
    }

    // Create and initialize game
    auto game = std::make_shared<InvasionCycle::Game>();
    if (!game->Initialize()) {
        return 1;
    }

    // Set game mode
    auto gameMode = std::make_shared<InvasionCycle::GameMode>();
    if (!gameMode->Initialize()) {
        return 1;
    }
    game->SetGameMode(gameMode);

    // Create player controller
    auto playerController = std::make_shared<InvasionCycle::ICPlayerController>();
    if (!playerController->Initialize()) {
        return 1;
    }
    game->AddPlayer(playerController);

    // Set game state to playing
    game->SetGameState(InvasionCycle::EGameState::Playing);

    // Run game loop
    app->Run(game);

    // Cleanup
    game->Shutdown();
    app->Shutdown();

    return 0;
} 