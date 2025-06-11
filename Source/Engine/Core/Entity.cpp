#include "Entity.h"
#include "Component.h"
#include "PlayerController.h"
#include <algorithm>

namespace InvasionEngine {

Entity::Entity()
    : m_Position(0.0f)
    , m_Rotation(0.0f)
    , m_Scale(1.0f)
    , m_Controller(nullptr)
{
}

Entity::~Entity()
{
    Shutdown();
}

bool Entity::Initialize()
{
    for (auto& component : m_Components) {
        if (!component->Initialize()) {
            return false;
        }
    }
    return true;
}

void Entity::Update(float deltaTime)
{
    for (auto& component : m_Components) {
        component->Update(deltaTime);
    }
}

void Entity::Render()
{
    for (auto& component : m_Components) {
        component->Render();
    }
}

void Entity::Shutdown()
{
    for (auto& component : m_Components) {
        component->Shutdown();
    }
    m_Components.clear();
}

void Entity::SetPosition(const Vector3& position)
{
    m_Position = position;
}

void Entity::SetRotation(const Vector3& rotation)
{
    m_Rotation = rotation;
}

void Entity::SetScale(const Vector3& scale)
{
    m_Scale = scale;
}

void Entity::AddComponent(std::shared_ptr<Component> component)
{
    if (component) {
        component->SetOwner(this);
        m_Components.push_back(component);
    }
}

void Entity::RemoveComponent(std::shared_ptr<Component> component)
{
    auto it = std::find(m_Components.begin(), m_Components.end(), component);
    if (it != m_Components.end()) {
        (*it)->Shutdown();
        m_Components.erase(it);
    }
}

template<typename T>
std::shared_ptr<T> Entity::GetComponent() const
{
    for (const auto& component : m_Components) {
        if (auto casted = std::dynamic_pointer_cast<T>(component)) {
            return casted;
        }
    }
    return nullptr;
}

void Entity::SetController(std::shared_ptr<PlayerController> controller)
{
    m_Controller = controller;
}

void Entity::AddChild(std::shared_ptr<Entity> child) {
    if (child && child != shared_from_this()) {
        child->SetParent(shared_from_this());
        m_Children.push_back(child);
    }
}

void Entity::RemoveChild(std::shared_ptr<Entity> child) {
    auto it = std::find(m_Children.begin(), m_Children.end(), child);
    if (it != m_Children.end()) {
        (*it)->SetParent(nullptr);
        m_Children.erase(it);
    }
}

void Entity::SetParent(std::shared_ptr<Entity> parent) {
    if (m_Parent) {
        m_Parent->RemoveChild(shared_from_this());
    }
    m_Parent = parent;
}

Matrix4x4 Entity::GetTransform() const {
    return Matrix4x4::Identity();
}

} // namespace InvasionEngine 