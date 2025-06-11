#include "Engine.h"
#include "Window.h"
#include "InputManager.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "AudioSystem.h"
#include <GLFW/glfw3.h>

namespace InvasionEngine {

Engine::Engine()
    : m_IsRunning(false)
    , m_LastFrameTime(0.0f)
{
}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize() {
    // Initialize window
    m_Window = std::make_unique<Window>();
    if (!m_Window->Initialize("Invasion Engine", 1280, 720)) {
        return false;
    }

    // Initialize input manager
    m_InputManager = std::make_unique<InputManager>();
    m_InputManager->Initialize();

    // Set up input callbacks
    m_Window->SetKeyCallback([this](int key, int scancode, int action, int mods) {
        m_InputManager->m_KeyStates[static_cast<Key>(key)] = (action != GLFW_RELEASE);
    });

    m_Window->SetMouseButtonCallback([this](int button, int action, int mods) {
        m_InputManager->m_MouseButtonStates[button] = (action != GLFW_RELEASE);
    });

    m_Window->SetMouseMoveCallback([this](double xpos, double ypos) {
        m_InputManager->m_MouseX = static_cast<float>(xpos);
        m_InputManager->m_MouseY = static_cast<float>(ypos);
    });

    // Initialize renderer
    m_Renderer = std::make_unique<Renderer>();
    if (!m_Renderer->Initialize()) {
        return false;
    }

    // Initialize scene manager
    m_SceneManager = std::make_unique<SceneManager>();
    if (!m_SceneManager->Initialize()) {
        return false;
    }

    // Initialize audio system
    m_AudioSystem = std::make_unique<AudioSystem>();
    if (!m_AudioSystem->Initialize()) {
        return false;
    }

    return true;
}

void Engine::Shutdown() {
    if (m_AudioSystem) {
        m_AudioSystem->Shutdown();
        m_AudioSystem.reset();
    }

    if (m_SceneManager) {
        m_SceneManager->Shutdown();
        m_SceneManager.reset();
    }

    if (m_Renderer) {
        m_Renderer->Shutdown();
        m_Renderer.reset();
    }

    if (m_InputManager) {
        m_InputManager->Shutdown();
        m_InputManager.reset();
    }

    if (m_Window) {
        m_Window->Shutdown();
        m_Window.reset();
    }
}

void Engine::Run() {
    m_IsRunning = true;
    m_LastFrameTime = static_cast<float>(glfwGetTime());

    while (m_IsRunning && !m_Window->ShouldClose()) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - m_LastFrameTime;
        m_LastFrameTime = currentTime;

        // Update input
        m_InputManager->Update();

        // Update game state
        m_SceneManager->Update(deltaTime);
        m_SceneManager->FixedUpdate(1.0f / 60.0f);
        m_SceneManager->LateUpdate(deltaTime);

        // Update audio
        m_AudioSystem->Update(deltaTime);

        // Render frame
        m_Renderer->BeginFrame();
        m_Renderer->Clear(0.2f, 0.3f, 0.3f, 1.0f);
        m_SceneManager->Render(m_Renderer.get());
        m_Renderer->EndFrame();

        // Swap buffers and poll events
        m_Window->SwapBuffers();
        m_Window->PollEvents();
    }
}

} // namespace InvasionEngine 