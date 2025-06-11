#include "BaseHUD.h"

namespace InvasionEngine {

BaseHUD::BaseHUD()
    : m_IsVisible(true)
{
}

void BaseHUD::Update(float deltaTime)
{
    // Update message durations
    for (auto it = m_Messages.begin(); it != m_Messages.end();) {
        it->second -= deltaTime;
        if (it->second <= 0.0f) {
            it = m_Messages.erase(it);
        } else {
            ++it;
        }
    }
}

void BaseHUD::FixedUpdate(float fixedDeltaTime)
{
    // Handle fixed update logic here
}

void BaseHUD::DrawHealthBar(float health, float maxHealth)
{
    // Override in derived classes
}

void BaseHUD::DrawAmmoCount(int currentAmmo, int maxAmmo)
{
    // Override in derived classes
}

void BaseHUD::DrawCrosshair()
{
    // Override in derived classes
}

void BaseHUD::DrawScore(int score)
{
    // Override in derived classes
}

void BaseHUD::DrawMessage(const std::string& message, float duration)
{
    m_Messages.emplace_back(message, duration);
}

void BaseHUD::ShowHUD()
{
    m_IsVisible = true;
}

void BaseHUD::HideHUD()
{
    m_IsVisible = false;
}

void BaseHUD::ToggleHUD()
{
    m_IsVisible = !m_IsVisible;
}

} // namespace InvasionEngine 