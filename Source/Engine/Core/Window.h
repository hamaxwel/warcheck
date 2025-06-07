#pragma once

#include <string>

namespace InvasionEngine {

class Window {
public:
    Window() = default;
    ~Window() = default;

    bool Initialize(const std::string& title, int width, int height);
    void Shutdown();

    bool ShouldClose() const;
    void PollEvents();
    void SwapBuffers();

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    void* GetNativeWindow() const { return m_Window; }

private:
    void* m_Window = nullptr;
    int m_Width = 0;
    int m_Height = 0;
    bool m_ShouldClose = false;
};

} // namespace InvasionEngine 