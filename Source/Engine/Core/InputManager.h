#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <vector>
#include "Math/Vector.h"

namespace InvasionEngine {

class Window;

enum class Key {
    Unknown = -1,
    Space = 32,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    Enter = 257,
    Escape = 256,
    Up = 265,
    Down = 264,
    Left = 263,
    Right = 262
};

enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2
};

class InputManager {
public:
    static InputManager& GetInstance() {
        static InputManager instance;
        return instance;
    }

    InputManager();
    ~InputManager();

    void Initialize();
    void Shutdown();
    void Update();

    bool IsKeyPressed(Key key) const;
    bool IsKeyJustPressed(Key key) const;
    bool IsKeyJustReleased(Key key) const;

    void GetMousePosition(float& x, float& y) const;
    void GetMouseDelta(float& dx, float& dy) const;
    bool IsMouseButtonPressed(int button) const;
    bool IsMouseButtonJustPressed(int button) const;
    bool IsMouseButtonJustReleased(int button) const;

    void SetKeyCallback(std::function<void(Key, bool)> callback);
    void SetMouseButtonCallback(std::function<void(int, bool)> callback);
    void SetMouseMoveCallback(std::function<void(float, float)> callback);

    float GetMouseX() const { return m_MouseX; }
    float GetMouseY() const { return m_MouseY; }

    // TODO: Add input binding and event system

private:
    std::unordered_map<Key, bool> m_KeyStates;
    std::unordered_map<Key, bool> m_PreviousKeyStates;
    std::unordered_map<int, bool> m_MouseButtonStates;
    std::unordered_map<int, bool> m_PreviousMouseButtonStates;
    float m_MouseX;
    float m_MouseY;
    float m_PreviousMouseX;
    float m_PreviousMouseY;
    std::function<void(Key, bool)> m_KeyCallback;
    std::function<void(int, bool)> m_MouseButtonCallback;
    std::function<void(float, float)> m_MouseMoveCallback;
};

} // namespace InvasionEngine 