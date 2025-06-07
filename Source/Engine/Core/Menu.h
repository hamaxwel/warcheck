#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "Math/Vector.h"

namespace InvasionEngine {

class MenuItem {
public:
    MenuItem(const std::string& text, const std::function<void()>& action);
    void Draw(const Vector2& position, bool isSelected) const;
    void Execute() const;

    const std::string& GetText() const { return m_Text; }
    void SetText(const std::string& text) { m_Text = text; }

private:
    std::string m_Text;
    std::function<void()> m_Action;
};

class Menu {
public:
    Menu();
    ~Menu() = default;

    void AddItem(const std::string& text, const std::function<void()>& action);
    void Update();
    void Draw() const;

    void MoveUp();
    void MoveDown();
    void Select();

    void Show() { m_IsVisible = true; }
    void Hide() { m_IsVisible = false; }
    bool IsVisible() const { return m_IsVisible; }

private:
    std::vector<MenuItem> m_Items;
    size_t m_SelectedIndex;
    bool m_IsVisible;
    Vector2 m_Position;
    float m_ItemSpacing;
};

} // namespace InvasionEngine 