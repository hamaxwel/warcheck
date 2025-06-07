#include "Core/Game.h"
#include <memory>

// Forward declare the game class
class InvasionCycleGame : public InvasionEngine::Game {
public:
    bool Initialize() override {
        // Set up game settings
        SetTitle("Invasion Cycle");
        SetWidth(1280);
        SetHeight(720);
        SetVSync(true);
        SetFullscreen(false);

        // Initialize game systems
        // TODO: Initialize game-specific systems

        m_IsRunning = true;
        return true;
    }

    void Shutdown() override {
        // TODO: Shutdown game-specific systems
    }

    void Update(float deltaTime) override {
        // TODO: Update game state
    }

    void Render() override {
        // TODO: Render game
    }
};

int main(int argc, char* argv[]) {
    // Create game instance
    auto game = std::make_shared<InvasionCycleGame>();

    // Create and run application
    InvasionEngine::GameApplication app;
    if (!app.Initialize(game)) {
        return -1;
    }

    app.Run();
    app.Shutdown();

    return 0;
} 