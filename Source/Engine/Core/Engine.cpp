#include "Engine.h"
#include "Window.h"
#include "Renderer.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "PhysicsSystem.h"
#include "AudioSystem.h"
#include <chrono>

namespace InvasionEngine {

Engine& Engine::GetInstance() {
    static Engine instance;
    return instance;
}

bool Engine::Initialize() {
    if (m_IsInitialized) return true;
    if (!InitializeSystems()) {
        return false;
    }

    m_IsRunning = true;
    m_LastFrameTime = 0.0f;
    m_IsInitialized = true;
    return true;
}

void Engine::Shutdown() {
    m_IsRunning = false;
    m_IsInitialized = false;
}

void Engine::Run() {
    while (m_IsRunning) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime.time_since_epoch()).count() - m_LastFrameTime;
        m_LastFrameTime = std::chrono::duration<float>(currentTime.time_since_epoch()).count();

        // Process input
        m_InputManager->Update();

        // Update game state
        Update(deltaTime);

        // Update physics
        m_PhysicsSystem->Update(deltaTime);

        // Render frame
        Render();

        // Check for window close
        if (m_Window->ShouldClose()) {
            m_IsRunning = false;
        }
    }
}

bool Engine::InitializeSystems() {
    // Create and initialize window
    m_Window = std::make_unique<Window>();
    if (!m_Window->Initialize("Invasion Cycle", 1280, 720)) {
        return false;
    }

    // Create and initialize renderer
    m_Renderer = std::make_unique<Renderer>();
    if (!m_Renderer->Initialize(m_Window.get())) {
        return false;
    }

    // Create and initialize input manager
    m_InputManager = std::make_unique<InputManager>();
    if (!m_InputManager->Initialize(m_Window.get())) {
        return false;
    }

    // Create and initialize resource manager
    m_ResourceManager = std::make_unique<ResourceManager>();
    if (!m_ResourceManager->Initialize()) {
        return false;
    }

    // Create and initialize scene manager
    m_SceneManager = std::make_unique<SceneManager>();
    if (!m_SceneManager->Initialize()) {
        return false;
    }

    // Create and initialize physics system
    m_PhysicsSystem = std::make_unique<PhysicsSystem>();
    if (!m_PhysicsSystem->Initialize()) {
        return false;
    }

    // Create and initialize audio system
    m_AudioSystem = std::make_unique<AudioSystem>();
    if (!m_AudioSystem->Initialize()) {
        return false;
    }

    return true;
}

void Engine::Update(float deltaTime) {
    m_DeltaTime = deltaTime;
    // Update scene
    m_SceneManager->Update(deltaTime);

    // Update audio
    m_AudioSystem->Update(deltaTime);
}

void Engine::Render() {
    m_Renderer->BeginFrame();
    m_SceneManager->Render(m_Renderer.get());
    m_Renderer->EndFrame();
}

void Engine::SetTargetFPS(int fps) {
    m_TargetFPS = fps;
    m_FixedDeltaTime = 1.0f / static_cast<float>(fps);
}

} // namespace InvasionEngine 