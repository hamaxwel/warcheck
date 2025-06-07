#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Mesh.h"
#include "CameraComponent.h"
#include "CameraControllerComponent.h"
#include "Math/Vector.h"
#include "HumanCharacter.h"
#include "AlienCharacter.h"
#include "AlienAIComponent.h"
#include "WeaponComponent.h"
#include "UIManager.h"
#include <memory>
#include <iostream>

namespace InvasionEngine {

SceneManager::SceneManager() : m_ActiveScene(nullptr) {}
SceneManager::~SceneManager() {}

void SceneManager::Initialize() {
    // Create default scene
    m_ActiveScene = CreateScene("DefaultScene");
    m_ActiveScene->Initialize();

    // Create ground plane
    auto ground = m_ActiveScene->CreateEntity("Ground");
    ground->SetPosition(Vector3(0.0f, -1.0f, 0.0f));
    Mesh* groundMesh = new Mesh();
    groundMesh->SetVertices({
        {{-10.0f, 0.0f, -10.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 10.0f, 0.0f, -10.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 10.0f, 0.0f,  10.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-10.0f, 0.0f,  10.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
    });
    groundMesh->SetIndices({0, 1, 2, 2, 3, 0});
    groundMesh->UploadToGPU();
    ground->AddComponent(std::shared_ptr<Component>(groundMesh));

    // Create player (human)
    auto player = m_ActiveScene->CreateEntity("Player");
    player->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    CameraComponent* camera = new CameraComponent();
    camera->SetPerspective(60.0f, 1.777f, 0.1f, 1000.0f);
    player->AddComponent(std::shared_ptr<Component>(camera));
    CameraControllerComponent* controller = new CameraControllerComponent();
    player->AddComponent(std::shared_ptr<Component>(controller));
    WeaponComponent* weapon = new WeaponComponent();
    player->AddComponent(std::shared_ptr<Component>(weapon));

    // Create several aliens
    for (int i = 0; i < 5; ++i) {
        auto alien = m_ActiveScene->CreateEntity("Alien" + std::to_string(i+1));
        alien->SetPosition(Vector3(-4.0f + i * 2.0f, 0.0f, 5.0f));
        AlienAIComponent* ai = new AlienAIComponent();
        ai->SetPatrolPoints({
            Vector3(-4.0f + i * 2.0f, 0.0f, 5.0f),
            Vector3(-4.0f + i * 2.0f, 0.0f, 7.0f),
            Vector3(-2.0f + i * 2.0f, 0.0f, 7.0f),
            Vector3(-2.0f + i * 2.0f, 0.0f, 5.0f)
        });
        alien->AddComponent(std::shared_ptr<Component>(ai));
    }
}

void SceneManager::Shutdown() {
    for (auto scene : m_Scenes) {
        scene->Shutdown();
        delete scene;
    }
    m_Scenes.clear();
    m_ActiveScene = nullptr;
}

void SceneManager::Update(float deltaTime) {
    if (m_ActiveScene) {
        m_ActiveScene->Update(deltaTime);
    }
}

void SceneManager::FixedUpdate(float fixedDeltaTime) {
    if (m_ActiveScene) {
        m_ActiveScene->FixedUpdate(fixedDeltaTime);
    }
}

void SceneManager::LateUpdate(float deltaTime) {
    if (m_ActiveScene) {
        m_ActiveScene->LateUpdate(deltaTime);
    }
}

void SceneManager::Reset() {
    Shutdown();
    Initialize();
}

Scene* SceneManager::CreateScene(const std::string& name) {
    Scene* scene = new Scene(name);
    m_Scenes.push_back(scene);
    return scene;
}

void SceneManager::DestroyScene(Scene* scene) {
    auto it = std::find(m_Scenes.begin(), m_Scenes.end(), scene);
    if (it != m_Scenes.end()) {
        if (m_ActiveScene == scene) {
            m_ActiveScene = nullptr;
        }
        scene->Shutdown();
        delete scene;
        m_Scenes.erase(it);
    }
}

Scene* SceneManager::GetScene(const std::string& name) {
    for (auto scene : m_Scenes) {
        if (scene->GetName() == name) {
            return scene;
        }
    }
    return nullptr;
}

void SceneManager::SetActiveScene(Scene* scene) {
    if (scene && std::find(m_Scenes.begin(), m_Scenes.end(), scene) != m_Scenes.end()) {
        m_ActiveScene = scene;
    }
}

} // namespace InvasionEngine 