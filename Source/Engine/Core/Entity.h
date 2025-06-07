#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Scene.h"
#include "Component.h"
#include "../Math/Vector.h"
#include "../Math/Quaternion.h"

namespace InvasionEngine {

class Scene;

class Entity {
public:
    Entity();
    virtual ~Entity();

    void AddComponent(std::shared_ptr<Component> component);
    void RemoveComponent(std::shared_ptr<Component> component);
    std::vector<std::shared_ptr<Component>>& GetComponents();

    const Vector3& GetPosition() const { return m_Position; }
    void SetPosition(const Vector3& position) { m_Position = position; }

    const Quaternion& GetRotation() const { return m_Rotation; }
    void SetRotation(const Quaternion& rotation) { m_Rotation = rotation; }

    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }

    Scene* GetScene() const { return m_Scene; }
    void SetScene(Scene* scene) { m_Scene = scene; }

    // TODO: Add transform, parent/child, and tag support

private:
    std::vector<std::shared_ptr<Component>> m_Components;
    Vector3 m_Position;
    Quaternion m_Rotation;
    std::string m_Name;
    Scene* m_Scene;
};

} // namespace InvasionEngine 