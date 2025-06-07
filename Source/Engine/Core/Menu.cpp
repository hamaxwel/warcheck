#include "Menu.h"
#include "InputManager.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>

namespace InvasionEngine {

MenuItem::MenuItem(const std::string& text, const std::function<void()>& action)
    : m_Text(text), m_Action(action) {}

void MenuItem::Draw(const Vector2& position, bool isSelected) const {
    // For now, just print to console
    if (isSelected) {
        std::cout << "> " << m_Text << std::endl;
    } else {
        std::cout << "  " << m_Text << std::endl;
    }
}

void MenuItem::Execute() const {
    if (m_Action) {
        m_Action();
    }
}

Menu::Menu() : m_SelectedIndex(0), m_IsVisible(false),
    m_Position(400.0f, 300.0f), m_ItemSpacing(30.0f) {}

void Menu::AddItem(const std::string& text, const std::function<void()>& action) {
    m_Items.emplace_back(text, action);
}

void Menu::Update() {
    if (!m_IsVisible) return;

    InputManager& input = InputManager::GetInstance();

    if (input.IsKeyPressed(Key::Up)) {
        MoveUp();
    }
    if (input.IsKeyPressed(Key::Down)) {
        MoveDown();
    }
    if (input.IsKeyPressed(Key::Enter)) {
        Select();
    }
}

void Menu::Draw() const {
    if (!m_IsVisible) return;

    // Draw menu background
    glDisable(GL_LIGHTING);
    glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(m_Position.x - 100.0f, m_Position.y - 50.0f);
    glVertex2f(m_Position.x + 100.0f, m_Position.y - 50.0f);
    glVertex2f(m_Position.x + 100.0f, m_Position.y + m_Items.size() * m_ItemSpacing + 50.0f);
    glVertex2f(m_Position.x - 100.0f, m_Position.y + m_Items.size() * m_ItemSpacing + 50.0f);
    glEnd();

    // Draw menu items
    for (size_t i = 0; i < m_Items.size(); i++) {
        Vector2 itemPos = m_Position + Vector2(0.0f, i * m_ItemSpacing);
        m_Items[i].Draw(itemPos, i == m_SelectedIndex);
    }

    glEnable(GL_LIGHTING);
}

void Menu::MoveUp() {
    if (m_SelectedIndex > 0) {
        m_SelectedIndex--;
    }
}

void Menu::MoveDown() {
    if (m_SelectedIndex < m_Items.size() - 1) {
        m_SelectedIndex++;
    }
}

void Menu::Select() {
    if (m_SelectedIndex < m_Items.size()) {
        m_Items[m_SelectedIndex].Execute();
    }
}

} // namespace InvasionEngine 