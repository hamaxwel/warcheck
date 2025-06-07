#include "UIManager.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>

namespace InvasionEngine {

UIManager& UIManager::GetInstance() {
    static UIManager instance;
    return instance;
}

UIManager::UIManager() : m_PlayerHealth(100.0f), m_PlayerAmmo(30), m_Message("") {}

void UIManager::SetPlayerHealth(float health) {
    m_PlayerHealth = health;
}

void UIManager::SetPlayerAmmo(int ammo) {
    m_PlayerAmmo = ammo;
}

void UIManager::ShowMessage(const std::string& message) {
    m_Message = message;
}

void UIManager::ClearMessage() {
    m_Message.clear();
}

void UIManager::Render() {
    // Placeholder: Render UI on screen using OpenGL
    // For now, just print to console
    std::cout << "[Health: " << m_PlayerHealth << "] [Ammo: " << m_PlayerAmmo << "]";
    if (!m_Message.empty()) {
        std::cout << " -- " << m_Message;
    }
    std::cout << std::endl;
}

} // namespace InvasionEngine 