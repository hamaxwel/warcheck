#pragma once

#include "Math/Matrix.h"
#include <memory>
#include <vector>
#include <string>

namespace InvasionEngine {

class Component;
class PlayerController;

class Entity : public std::enable_shared_from_this<Entity> {
public:
    Entity();
    virtual ~Entity() = default;

    // Core functionality
    virtual bool Initialize();
    virtual void Update(float deltaTime);
    virtual void Render();
    virtual void Shutdown();

    // Transform
    void SetPosition(const Vector3& position);
    void SetRotation(const Vector3& rotation);
    void SetScale(const Vector3& scale);
    Vector3 GetPosition() const { return m_Position; }
    Vector3 GetRotation() const { return m_Rotation; }
    Vector3 GetScale() const { return m_Scale; }

    // Component management
    template<typename T>
    std::shared_ptr<T> GetComponent() const {
        for (const auto& component : m_Components) {
            if (auto casted = std::dynamic_pointer_cast<T>(component)) {
                return casted;
            }
        }
        return nullptr;
    }

    template<typename T>
    void AddComponent(std::shared_ptr<T> component) {
        if (component) {
            component->SetOwner(shared_from_this());
            m_Components.push_back(component);
        }
    }

    void RemoveComponent(std::shared_ptr<Component> component);

    // Entity hierarchy
    void AddChild(std::shared_ptr<Entity> child);
    void RemoveChild(std::shared_ptr<Entity> child);
    void SetParent(std::shared_ptr<Entity> parent);
    std::shared_ptr<Entity> GetParent() const { return m_Parent; }
    const std::vector<std::shared_ptr<Entity>>& GetChildren() const { return m_Children; }

    // Properties
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }
    bool IsActive() const { return m_IsActive; }
    void SetActive(bool active) { m_IsActive = active; }

    void SetController(std::shared_ptr<PlayerController> controller);
    std::shared_ptr<PlayerController> GetController() const { return m_Controller; }

protected:
    std::string m_Name;
    bool m_IsActive;
    Vector3 m_Position;
    Vector3 m_Rotation;
    Vector3 m_Scale;
    std::vector<std::shared_ptr<Component>> m_Components;
    std::shared_ptr<Entity> m_Parent;
    std::vector<std::shared_ptr<Entity>> m_Children;
    std::shared_ptr<PlayerController> m_Controller;
};

} // namespace InvasionEngine 