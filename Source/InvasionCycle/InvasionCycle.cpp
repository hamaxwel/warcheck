#include "InvasionCycle.h"
#include "Engine/Core/Engine.h"
#include <iostream>

// Initialize the game engine
InvasionEngine::Engine* g_Engine = nullptr;

int main(int argc, char* argv[]) {
    try {
        // Create and initialize the engine
        g_Engine = new InvasionEngine::Engine();
        if (!g_Engine->Initialize()) {
            std::cerr << "Failed to initialize engine" << std::endl;
            return 1;
        }

        // Create and initialize the game mode
        auto gameMode = std::make_shared<InvasionCycle::GameMode>();
        if (!gameMode->Initialize()) {
            std::cerr << "Failed to initialize game mode" << std::endl;
            return 1;
        }

        // Run the main game loop
        while (g_Engine->IsRunning()) {
            float deltaTime = g_Engine->GetDeltaTime();
            gameMode->Update(deltaTime);
            gameMode->Render();
        }

        // Cleanup
        gameMode->Shutdown();
        delete g_Engine;
        g_Engine = nullptr;

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
} 