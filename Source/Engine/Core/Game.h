#pragma once

#include <string>
#include <memory>

namespace InvasionEngine {

class Game {
public:
    Game() = default;
    virtual ~Game() = default;

    // Core game lifecycle methods
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;

    // Window settings
    void SetTitle(const std::string& title) { m_Title = title; }
    void SetWidth(int width) { m_Width = width; }
    void SetHeight(int height) { m_Height = height; }
    void SetVSync(bool vsync) { m_VSync = vsync; }
    void SetFullscreen(bool fullscreen) { m_Fullscreen = fullscreen; }

    // Getters
    const std::string& GetTitle() const { return m_Title; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    bool IsVSyncEnabled() const { return m_VSync; }
    bool IsFullscreen() const { return m_Fullscreen; }
    bool IsRunning() const { return m_IsRunning; }

protected:
    std::string m_Title = "Game";
    int m_Width = 1280;
    int m_Height = 720;
    bool m_VSync = true;
    bool m_Fullscreen = false;
    bool m_IsRunning = false;
};

} // namespace InvasionEngine 