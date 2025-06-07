#pragma once

#include <memory>
#include <string>
#include "Engine.h"

namespace InvasionEngine {

class Game {
public:
    Game() = default;
    virtual ~Game() = default;

    // Game lifecycle
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    // Game settings
    void SetTitle(const std::string& title) { m_Title = title; }
    void SetWidth(int width) { m_Width = width; }
    void SetHeight(int height) { m_Height = height; }
    void SetVSync(bool vsync) { m_VSync = vsync; }
    void SetFullscreen(bool fullscreen) { m_Fullscreen = fullscreen; }

    const std::string& GetTitle() const { return m_Title; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    bool GetVSync() const { return m_VSync; }
    bool GetFullscreen() const { return m_Fullscreen; }

    // Game state
    bool IsRunning() const { return m_IsRunning; }
    void Quit() { m_IsRunning = false; }

protected:
    std::string m_Title = "Invasion Cycle";
    int m_Width = 1280;
    int m_Height = 720;
    bool m_VSync = true;
    bool m_Fullscreen = false;
    bool m_IsRunning = false;
};

class GameApplication {
public:
    GameApplication() = default;
    ~GameApplication() = default;

    bool Initialize(std::shared_ptr<Game> game);
    void Shutdown();
    void Run();

private:
    std::shared_ptr<Game> m_Game;
    Engine& m_Engine = Engine::GetInstance();
};

} // namespace InvasionEngine 