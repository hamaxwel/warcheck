#include "InputManager.h"
#include "Window.h"

namespace InvasionEngine {

InputManager::InputManager()
    : m_MouseX(0.0f)
    , m_MouseY(0.0f)
    , m_PreviousMouseX(0.0f)
    , m_PreviousMouseY(0.0f)
{
}

InputManager::~InputManager() {
}

void InputManager::Initialize() {
    // Initialize input state
    for (int i = 0; i < 348; i++) {
        m_KeyStates[static_cast<Key>(i)] = false;
        m_PreviousKeyStates[static_cast<Key>(i)] = false;
    }
}

void InputManager::Shutdown() {
    m_KeyStates.clear();
    m_PreviousKeyStates.clear();
    m_MouseButtonStates.clear();
    m_PreviousMouseButtonStates.clear();
}

void InputManager::Update() {
    // Update previous states
    m_PreviousKeyStates = m_KeyStates;
    m_PreviousMouseButtonStates = m_MouseButtonStates;
    m_PreviousMouseX = m_MouseX;
    m_PreviousMouseY = m_MouseY;
}

bool InputManager::IsKeyPressed(Key key) const {
    auto it = m_KeyStates.find(key);
    return it != m_KeyStates.end() && it->second;
}

bool InputManager::IsKeyJustPressed(Key key) const {
    auto it = m_KeyStates.find(key);
    auto prevIt = m_PreviousKeyStates.find(key);
    return it != m_KeyStates.end() && prevIt != m_PreviousKeyStates.end() &&
           it->second && !prevIt->second;
}

bool InputManager::IsKeyJustReleased(Key key) const {
    auto it = m_KeyStates.find(key);
    auto prevIt = m_PreviousKeyStates.find(key);
    return it != m_KeyStates.end() && prevIt != m_PreviousKeyStates.end() &&
           !it->second && prevIt->second;
}

void InputManager::GetMousePosition(float& x, float& y) const {
    x = m_MouseX;
    y = m_MouseY;
}

void InputManager::GetMouseDelta(float& dx, float& dy) const {
    dx = m_MouseX - m_PreviousMouseX;
    dy = m_MouseY - m_PreviousMouseY;
}

bool InputManager::IsMouseButtonPressed(int button) const {
    auto it = m_MouseButtonStates.find(button);
    return it != m_MouseButtonStates.end() && it->second;
}

bool InputManager::IsMouseButtonJustPressed(int button) const {
    auto it = m_MouseButtonStates.find(button);
    auto prevIt = m_PreviousMouseButtonStates.find(button);
    return it != m_MouseButtonStates.end() && prevIt != m_PreviousMouseButtonStates.end() &&
           it->second && !prevIt->second;
}

bool InputManager::IsMouseButtonJustReleased(int button) const {
    auto it = m_MouseButtonStates.find(button);
    auto prevIt = m_PreviousMouseButtonStates.find(button);
    return it != m_MouseButtonStates.end() && prevIt != m_PreviousMouseButtonStates.end() &&
           !it->second && prevIt->second;
}

void InputManager::SetKeyCallback(std::function<void(Key, bool)> callback) {
    m_KeyCallback = callback;
}

void InputManager::SetMouseButtonCallback(std::function<void(int, bool)> callback) {
    m_MouseButtonCallback = callback;
}

void InputManager::SetMouseMoveCallback(std::function<void(float, float)> callback) {
    m_MouseMoveCallback = callback;
}

} // namespace InvasionEngine 