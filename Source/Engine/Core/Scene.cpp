#include "Scene.h"
#include "Entity.h"
#include "Mesh.h"
#include "CameraComponent.h"
#include "CameraControllerComponent.h"
#include "../Math/Vector.h"
#include <memory>
#include <iostream>

namespace InvasionEngine {

Scene::Scene(const std::string& name) : m_Name(name) {}
Scene::~Scene() {}

void Scene::Initialize() {
    // Initialize scene data
    m_Entities.clear();
    m_EntityMap.clear();
    m_Components.clear();
    m_Parents.clear();
    m_Children.clear();
    m_Lights.clear();
    m_ActiveCamera = nullptr;
}

void Scene::Shutdown() {
    // Clean up scene data
    m_Entities.clear();
    m_EntityMap.clear();
    m_Components.clear();
    m_Parents.clear();
    m_Children.clear();
    m_Lights.clear();
    m_ActiveCamera = nullptr;
}

void Scene::Update(float deltaTime) {
    // Update all entities and components
    for (auto& entity : m_Entities) {
        for (auto& component : entity->GetComponents()) {
            component->Update(deltaTime);
        }
    }

    // Update scene systems
    UpdateTransforms();
    UpdateLights();
    UpdateCameras();
    UpdatePhysics();
    UpdateAudio();
}

void Scene::FixedUpdate(float fixedDeltaTime) {
    // Update physics and other fixed timestep systems
    for (auto& entity : m_Entities) {
        for (auto& component : entity->GetComponents()) {
            component->FixedUpdate(fixedDeltaTime);
        }
    }
}

void Scene::LateUpdate(float deltaTime) {
    // Update systems that need to run after all other updates
    for (auto& entity : m_Entities) {
        for (auto& component : entity->GetComponents()) {
            component->LateUpdate(deltaTime);
        }
    }
}

void Scene::Reset() {
    Shutdown();
    Initialize();
}

std::shared_ptr<Entity> Scene::CreateEntity(const std::string& name) {
    auto entity = std::make_shared<Entity>();
    entity->SetName(name);
    entity->SetScene(this);
    m_Entities.push_back(entity);
    m_EntityMap[name] = entity;
    return entity;
}

void Scene::DestroyEntity(std::shared_ptr<Entity> entity) {
    // Remove from parent/child relationships
    RemoveParent(entity);
    auto children = GetChildren(entity);
    for (auto& child : children) {
        RemoveParent(child);
    }

    // Remove from entity lists
    m_Entities.erase(
        std::remove(m_Entities.begin(), m_Entities.end(), entity),
        m_Entities.end()
    );
    m_EntityMap.erase(entity->GetName());

    // Remove all components
    for (auto& component : entity->GetComponents()) {
        auto typeIndex = std::type_index(typeid(*component));
        auto& components = m_Components[typeIndex];
        components.erase(
            std::remove(components.begin(), components.end(), component),
            components.end()
        );
    }
}

std::shared_ptr<Entity> Scene::GetEntity(const std::string& name) const {
    auto it = m_EntityMap.find(name);
    if (it != m_EntityMap.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Entity>> Scene::GetEntities() const {
    return m_Entities;
}

void Scene::SetActiveCamera(std::shared_ptr<Camera> camera) {
    m_ActiveCamera = camera;
}

void Scene::AddLight(std::shared_ptr<Light> light) {
    m_Lights.push_back(light);
}

void Scene::RemoveLight(std::shared_ptr<Light> light) {
    m_Lights.erase(
        std::remove(m_Lights.begin(), m_Lights.end(), light),
        m_Lights.end()
    );
}

void Scene::SetParent(std::shared_ptr<Entity> child, std::shared_ptr<Entity> parent) {
    // Remove existing parent
    RemoveParent(child);

    // Set new parent
    if (parent) {
        m_Parents[child] = parent;
        m_Children[parent].push_back(child);
    }
}

void Scene::RemoveParent(std::shared_ptr<Entity> child) {
    auto parentIt = m_Parents.find(child);
    if (parentIt != m_Parents.end()) {
        auto parent = parentIt->second;
        auto& children = m_Children[parent];
        children.erase(
            std::remove(children.begin(), children.end(), child),
            children.end()
        );
        m_Parents.erase(parentIt);
    }
}

std::shared_ptr<Entity> Scene::GetParent(std::shared_ptr<Entity> entity) const {
    auto it = m_Parents.find(entity);
    if (it != m_Parents.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Entity>> Scene::GetChildren(std::shared_ptr<Entity> entity) const {
    auto it = m_Children.find(entity);
    if (it != m_Children.end()) {
        return it->second;
    }
    return std::vector<std::shared_ptr<Entity>>();
}

std::vector<std::shared_ptr<Entity>> Scene::FindEntitiesInRadius(const Vector3& center, float radius) const {
    std::vector<std::shared_ptr<Entity>> result;
    float radiusSquared = radius * radius;
    for (const auto& entity : m_Entities) {
        Vector3 diff = entity->GetPosition() - center;
        if (diff.LengthSquared() <= radiusSquared) {
            result.push_back(entity);
        }
    }
    return result;
}

std::vector<std::shared_ptr<Entity>> Scene::FindEntitiesInBox(const Vector3& min, const Vector3& max) const {
    std::vector<std::shared_ptr<Entity>> result;
    for (const auto& entity : m_Entities) {
        const Vector3& pos = entity->GetPosition();
        if (pos.x >= min.x && pos.x <= max.x &&
            pos.y >= min.y && pos.y <= max.y &&
            pos.z >= min.z && pos.z <= max.z) {
            result.push_back(entity);
        }
    }
    return result;
}

std::shared_ptr<Entity> Scene::Raycast(const Vector3& origin, const Vector3& direction, float maxDistance) const {
    std::shared_ptr<Entity> closestEntity = nullptr;
    float closestDistance = maxDistance;

    for (const auto& entity : m_Entities) {
        // TODO: Implement proper ray-mesh intersection
        // For now, just check distance to entity center
        Vector3 diff = entity->GetPosition() - origin;
        float distance = diff.Length();
        if (distance < closestDistance) {
            closestEntity = entity;
            closestDistance = distance;
        }
    }

    return closestEntity;
}

void Scene::UpdateTransforms() {
    // Update entity transforms based on hierarchy
    for (const auto& entity : m_Entities) {
        auto parent = GetParent(entity);
        if (parent) {
            // TODO: Update transform based on parent
        }
    }
}

void Scene::UpdateLights() {
    // Update light positions and properties
    for (const auto& light : m_Lights) {
        // TODO: Update light properties
    }
}

void Scene::UpdateCameras() {
    // Update camera properties
    if (m_ActiveCamera) {
        // TODO: Update camera properties
    }
}

void Scene::UpdatePhysics() {
    // Update physics simulation
    // TODO: Implement physics update
}

void Scene::UpdateAudio() {
    // Update audio sources and listeners
    // TODO: Implement audio update
}

} // namespace InvasionEngine 