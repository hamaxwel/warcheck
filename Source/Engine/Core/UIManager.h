#pragma once
#include <string>

namespace InvasionEngine {

class UIManager {
public:
    static UIManager& GetInstance();

    void SetPlayerHealth(float health);
    void SetPlayerAmmo(int ammo);
    void ShowMessage(const std::string& message);
    void ClearMessage();

    void Render();

    const std::string& GetMessage() const { return m_Message; }

private:
    UIManager();
    float m_PlayerHealth;
    int m_PlayerAmmo;
    std::string m_Message;
};

} // namespace InvasionEngine 