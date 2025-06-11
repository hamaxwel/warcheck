#pragma once

#include <string>
#include <functional>

namespace InvasionEngine {

class Window {
public:
    Window();
    ~Window();

    bool Initialize(const std::string& title, int width, int height);
    void Shutdown();
    void Update();
    void PollEvents();
    void SwapBuffers();
    bool ShouldClose() const;

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    float GetAspectRatio() const { return static_cast<float>(m_Width) / static_cast<float>(m_Height); }

    void SetKeyCallback(std::function<void(int, int, int, int)> callback);
    void SetMouseButtonCallback(std::function<void(int, int, int)> callback);
    void SetMouseMoveCallback(std::function<void(double, double)> callback);
    void SetScrollCallback(std::function<void(double, double)> callback);
    void SetResizeCallback(std::function<void(int, int)> callback);

private:
    void* m_Window;
    int m_Width;
    int m_Height;
    std::string m_Title;
    bool m_ShouldClose;
};

} // namespace InvasionEngine 