#pragma once

#include "Game.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

namespace InvasionEngine {

class GameApplication {
public:
    GameApplication();
    virtual ~GameApplication();

    bool Initialize(const std::string& title, int width, int height);
    void Run(std::shared_ptr<Game> game);
    void Shutdown();

    // Window management
    GLFWwindow* GetWindow() const { return m_Window; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    bool IsRunning() const { return m_IsRunning; }

protected:
    // Window callbacks
    static void OnWindowResize(GLFWwindow* window, int width, int height);
    static void OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
    static void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
    static void OnScroll(GLFWwindow* window, double xoffset, double yoffset);

    // Input handling
    void ProcessInput();

private:
    GLFWwindow* m_Window;
    int m_Width;
    int m_Height;
    bool m_IsRunning;
    std::shared_ptr<Game> m_Game;
};

} // namespace InvasionEngine 