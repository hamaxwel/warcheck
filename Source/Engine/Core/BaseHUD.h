#pragma once

#include "Component.h"
#include <memory>
#include <vector>
#include <string>

namespace InvasionEngine {

class BaseHUD : public Component {
public:
    BaseHUD();
    virtual ~BaseHUD() = default;

    virtual void Update(float deltaTime) override;
    virtual void FixedUpdate(float fixedDeltaTime) override;

    // UI Elements
    virtual void DrawHealthBar(float health, float maxHealth);
    virtual void DrawAmmoCount(int currentAmmo, int maxAmmo);
    virtual void DrawCrosshair();
    virtual void DrawScore(int score);
    virtual void DrawMessage(const std::string& message, float duration = 2.0f);

    // HUD State
    virtual void ShowHUD();
    virtual void HideHUD();
    virtual void ToggleHUD();

    // Getters/Setters
    bool IsVisible() const { return m_IsVisible; }
    void SetVisible(bool visible) { m_IsVisible = visible; }

protected:
    bool m_IsVisible;
    std::vector<std::pair<std::string, float>> m_Messages; // Message and remaining duration
};

} // namespace InvasionEngine 