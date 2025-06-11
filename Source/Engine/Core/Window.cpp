#include "Window.h"
#include <GLFW/glfw3.h>

namespace InvasionEngine {

Window::Window()
    : m_Window(nullptr)
    , m_Width(0)
    , m_Height(0)
    , m_ShouldClose(false)
{
}

Window::~Window() {
    Shutdown();
}

bool Window::Initialize(const std::string& title, int width, int height) {
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window));
    glfwSwapInterval(1);

    m_Width = width;
    m_Height = height;
    m_Title = title;

    return true;
}

void Window::Shutdown() {
    if (m_Window) {
        glfwDestroyWindow(static_cast<GLFWwindow*>(m_Window));
        m_Window = nullptr;
    }
    glfwTerminate();
}

void Window::Update() {
    if (m_Window) {
        glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window));
    }
}

void Window::PollEvents() {
    glfwPollEvents();
}

void Window::SwapBuffers() {
    if (m_Window) {
        glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window));
    }
}

bool Window::ShouldClose() const {
    return m_Window ? glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window)) : true;
}

void Window::SetKeyCallback(std::function<void(int, int, int, int)> callback) {
    if (m_Window) {
        glfwSetKeyCallback(static_cast<GLFWwindow*>(m_Window), 
            [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                auto callback = static_cast<std::function<void(int, int, int, int)>*>(
                    glfwGetWindowUserPointer(window));
                if (callback) {
                    (*callback)(key, scancode, action, mods);
                }
            });
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_Window), &callback);
    }
}

void Window::SetMouseButtonCallback(std::function<void(int, int, int)> callback) {
    if (m_Window) {
        glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* window, int button, int action, int mods) {
                auto callback = static_cast<std::function<void(int, int, int)>*>(
                    glfwGetWindowUserPointer(window));
                if (callback) {
                    (*callback)(button, action, mods);
                }
            });
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_Window), &callback);
    }
}

void Window::SetMouseMoveCallback(std::function<void(double, double)> callback) {
    if (m_Window) {
        glfwSetCursorPosCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* window, double xpos, double ypos) {
                auto callback = static_cast<std::function<void(double, double)>*>(
                    glfwGetWindowUserPointer(window));
                if (callback) {
                    (*callback)(xpos, ypos);
                }
            });
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_Window), &callback);
    }
}

void Window::SetScrollCallback(std::function<void(double, double)> callback) {
    if (m_Window) {
        glfwSetScrollCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* window, double xoffset, double yoffset) {
                auto callback = static_cast<std::function<void(double, double)>*>(
                    glfwGetWindowUserPointer(window));
                if (callback) {
                    (*callback)(xoffset, yoffset);
                }
            });
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_Window), &callback);
    }
}

void Window::SetResizeCallback(std::function<void(int, int)> callback) {
    if (m_Window) {
        glfwSetFramebufferSizeCallback(static_cast<GLFWwindow*>(m_Window),
            [](GLFWwindow* window, int width, int height) {
                auto callback = static_cast<std::function<void(int, int)>*>(
                    glfwGetWindowUserPointer(window));
                if (callback) {
                    (*callback)(width, height);
                }
            });
        glfwSetWindowUserPointer(static_cast<GLFWwindow*>(m_Window), &callback);
    }
}

} // namespace InvasionEngine 