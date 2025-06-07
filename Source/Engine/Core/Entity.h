#pragma once

#include <vector>
#include <memory>
#include "Component.h"

namespace InvasionEngine {

class Entity {
public:
    Entity();
    virtual ~Entity();

    void AddComponent(std::shared_ptr<Component> component);
    void RemoveComponent(std::shared_ptr<Component> component);
    std::vector<std::shared_ptr<Component>>& GetComponents();

    // TODO: Add transform, parent/child, and tag support

private:
    std::vector<std::shared_ptr<Component>> m_Components;
};

} // namespace InvasionEngine 