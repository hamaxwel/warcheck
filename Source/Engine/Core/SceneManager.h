#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Scene.h"

namespace InvasionEngine {

class Entity;
class Component;
class Camera;
class Light;
class Mesh;
class Material;
class Renderer;

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    void Initialize();
    void Shutdown();
    void Update(float deltaTime);
    void FixedUpdate(float fixedDeltaTime);
    void LateUpdate(float deltaTime);
    void Render(Renderer* renderer);
    void Reset();

    // TODO: Add methods for scene loading/unloading

    // Scene management
    Scene* CreateScene(const std::string& name);
    void DestroyScene(Scene* scene);
    Scene* GetScene(const std::string& name);
    Scene* GetActiveScene() const { return m_ActiveScene; }
    void SetActiveScene(Scene* scene);

    // Entity management
    std::shared_ptr<Entity> CreateEntity(const std::string& name = "Entity");
    void DestroyEntity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> GetEntity(const std::string& name) const;
    std::vector<std::shared_ptr<Entity>> GetEntities() const;

    // Component management
    template<typename T>
    std::shared_ptr<T> AddComponent(std::shared_ptr<Entity> entity);
    template<typename T>
    void RemoveComponent(std::shared_ptr<Entity> entity);
    template<typename T>
    std::shared_ptr<T> GetComponent(std::shared_ptr<Entity> entity) const;
    template<typename T>
    std::vector<std::shared_ptr<T>> GetComponents() const;

    // Camera management
    void SetActiveCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> GetActiveCamera() const { return m_ActiveCamera; }

    // Light management
    void AddLight(std::shared_ptr<Light> light);
    void RemoveLight(std::shared_ptr<Light> light);
    std::vector<std::shared_ptr<Light>> GetLights() const { return m_Lights; }

    // Scene hierarchy
    void SetParent(std::shared_ptr<Entity> child, std::shared_ptr<Entity> parent);
    void RemoveParent(std::shared_ptr<Entity> child);
    std::shared_ptr<Entity> GetParent(std::shared_ptr<Entity> entity) const;
    std::vector<std::shared_ptr<Entity>> GetChildren(std::shared_ptr<Entity> entity) const;

    // Scene queries
    std::vector<std::shared_ptr<Entity>> FindEntitiesInRadius(const Vector3& center, float radius) const;
    std::vector<std::shared_ptr<Entity>> FindEntitiesInBox(const Vector3& min, const Vector3& max) const;
    std::shared_ptr<Entity> Raycast(const Vector3& origin, const Vector3& direction, float maxDistance) const;

private:
    // Internal scene data
    std::vector<std::shared_ptr<Entity>> m_Entities;
    std::unordered_map<std::string, std::shared_ptr<Entity>> m_EntityMap;
    std::shared_ptr<Camera> m_ActiveCamera;
    std::vector<std::shared_ptr<Light>> m_Lights;

    // Component storage
    std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>> m_Components;

    // Scene hierarchy
    std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<Entity>> m_Parents;
    std::unordered_map<std::shared_ptr<Entity>, std::vector<std::shared_ptr<Entity>>> m_Children;

    // Helper functions
    void UpdateTransforms();
    void UpdateLights();
    void UpdateCameras();
    void UpdatePhysics();
    void UpdateAudio();

    std::unordered_map<std::type_index, std::vector<Entity*>> m_EntitiesByType;
    std::vector<Scene*> m_Scenes;
    Scene* m_ActiveScene;
};

// Template implementations
template<typename T>
std::shared_ptr<T> SceneManager::AddComponent(std::shared_ptr<Entity> entity) {
    auto component = std::make_shared<T>();
    m_Components[std::type_index(typeid(T))].push_back(component);
    return component;
}

template<typename T>
void SceneManager::RemoveComponent(std::shared_ptr<Entity> entity) {
    auto& components = m_Components[std::type_index(typeid(T))];
    components.erase(
        std::remove_if(components.begin(), components.end(),
            [entity](const std::shared_ptr<Component>& component) {
                return component->GetEntity() == entity;
            }),
        components.end()
    );
}

template<typename T>
std::shared_ptr<T> SceneManager::GetComponent(std::shared_ptr<Entity> entity) const {
    auto it = m_Components.find(std::type_index(typeid(T)));
    if (it != m_Components.end()) {
        for (const auto& component : it->second) {
            if (component->GetEntity() == entity) {
                return std::static_pointer_cast<T>(component);
            }
        }
    }
    return nullptr;
}

template<typename T>
std::vector<std::shared_ptr<T>> SceneManager::GetComponents() const {
    std::vector<std::shared_ptr<T>> result;
    auto it = m_Components.find(std::type_index(typeid(T)));
    if (it != m_Components.end()) {
        for (const auto& component : it->second) {
            result.push_back(std::static_pointer_cast<T>(component));
        }
    }
    return result;
}

} // namespace InvasionEngine 