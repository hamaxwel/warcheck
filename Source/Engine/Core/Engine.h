#pragma once

#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include "Window.h"
#include "Renderer.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "PhysicsSystem.h"
#include "AudioSystem.h"

namespace InvasionEngine {

class Window;
class InputManager;
class Renderer;
class SceneManager;
class AudioSystem;

class Engine {
public:
    Engine();
    ~Engine();

    bool Initialize();
    void Shutdown();
    void Run();
    void Update(float deltaTime);
    void Render();

    // Getters for subsystems
    Window& GetWindow() { return *m_Window; }
    Renderer& GetRenderer() { return *m_Renderer; }
    InputManager& GetInputManager() { return *m_InputManager; }
    ResourceManager& GetResourceManager() { return *m_ResourceManager; }
    SceneManager& GetSceneManager() { return *m_SceneManager; }
    PhysicsSystem& GetPhysicsSystem() { return *m_PhysicsSystem; }
    AudioSystem& GetAudioSystem() { return *m_AudioSystem; }

    // Engine settings
    void SetTargetFPS(int fps);
    int GetTargetFPS() const { return m_TargetFPS; }
    float GetDeltaTime() const { return m_DeltaTime; }
    float GetFixedDeltaTime() const { return m_FixedDeltaTime; }

private:
    bool InitializeSystems();

    // Subsystems
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<InputManager> m_InputManager;
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<SceneManager> m_SceneManager;
    std::unique_ptr<AudioSystem> m_AudioSystem;

    // Engine state
    bool m_IsInitialized = false;
    bool m_IsRunning = false;
    int m_TargetFPS = 60;
    float m_DeltaTime = 0.0f;
    float m_FixedDeltaTime = 1.0f / 60.0f;
    float m_Accumulator = 0.0f;
    float m_LastFrameTime = 0.0f;
};

} // namespace InvasionEngine 