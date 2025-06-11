#include "GameApplication.h"
#include <iostream>

namespace InvasionEngine {

GameApplication::GameApplication()
    : m_Window(nullptr)
    , m_Width(0)
    , m_Height(0)
    , m_IsRunning(false)
{
}

GameApplication::~GameApplication()
{
    Shutdown();
}

bool GameApplication::Initialize(const std::string& title, int width, int height)
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set window properties
    m_Width = width;
    m_Height = height;
    m_IsRunning = true;

    // Make the window's context current
    glfwMakeContextCurrent(m_Window);

    // Set callbacks
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, OnWindowResize);
    glfwSetKeyCallback(m_Window, OnKeyPress);
    glfwSetCursorPosCallback(m_Window, OnMouseMove);
    glfwSetMouseButtonCallback(m_Window, OnMouseButton);
    glfwSetScrollCallback(m_Window, OnScroll);

    // Enable vsync
    glfwSwapInterval(1);

    return true;
}

void GameApplication::Run(std::shared_ptr<Game> game)
{
    m_Game = game;
    if (!m_Game || !m_Game->Initialize()) {
        std::cerr << "Failed to initialize game" << std::endl;
        return;
    }

    float lastFrameTime = static_cast<float>(glfwGetTime());

    // Main game loop
    while (!glfwWindowShouldClose(m_Window) && m_IsRunning) {
        // Calculate delta time
        float currentFrameTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // Process input
        ProcessInput();

        // Update game state
        m_Game->Update(deltaTime);

        // Render
        m_Game->Render();

        // Swap buffers and poll events
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    m_Game->Shutdown();
}

void GameApplication::Shutdown()
{
    if (m_Game) {
        m_Game->Shutdown();
        m_Game.reset();
    }

    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    glfwTerminate();
    m_IsRunning = false;
}

void GameApplication::OnWindowResize(GLFWwindow* window, int width, int height)
{
    auto app = static_cast<GameApplication*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->m_Width = width;
        app->m_Height = height;
        glViewport(0, 0, width, height);
    }
}

void GameApplication::OnKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto app = static_cast<GameApplication*>(glfwGetWindowUserPointer(window));
    if (app && app->m_Game) {
        // TODO: Implement key input handling
    }
}

void GameApplication::OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    auto app = static_cast<GameApplication*>(glfwGetWindowUserPointer(window));
    if (app && app->m_Game) {
        // TODO: Implement mouse movement handling
    }
}

void GameApplication::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
{
    auto app = static_cast<GameApplication*>(glfwGetWindowUserPointer(window));
    if (app && app->m_Game) {
        // TODO: Implement mouse button handling
    }
}

void GameApplication::OnScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    auto app = static_cast<GameApplication*>(glfwGetWindowUserPointer(window));
    if (app && app->m_Game) {
        // TODO: Implement scroll handling
    }
}

void GameApplication::ProcessInput()
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        m_IsRunning = false;
    }
}

} // namespace InvasionEngine 