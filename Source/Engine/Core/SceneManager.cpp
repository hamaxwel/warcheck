#include "SceneManager.h"
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

SceneManager::SceneManager() {}
SceneManager::~SceneManager() {}

void SceneManager::Initialize() {
    // Create ground plane
    Entity* ground = new Entity();
    ground->SetName("Ground");
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
    m_Entities.push_back(std::shared_ptr<Entity>(ground));

    // Create player (human)
    HumanCharacter* player = new HumanCharacter();
    player->SetName("Player");
    player->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
    CameraComponent* camera = new CameraComponent();
    camera->SetPerspective(60.0f, 1.777f, 0.1f, 1000.0f);
    player->AddComponent(std::shared_ptr<Component>(camera));
    CameraControllerComponent* controller = new CameraControllerComponent();
    player->AddComponent(std::shared_ptr<Component>(controller));
    WeaponComponent* weapon = new WeaponComponent();
    player->AddComponent(std::shared_ptr<Component>(weapon));
    m_Entities.push_back(std::shared_ptr<Entity>(player));

    // Create several aliens
    for (int i = 0; i < 5; ++i) {
        AlienCharacter* alien = new AlienCharacter();
        alien->SetName("Alien" + std::to_string(i+1));
        alien->SetPosition(Vector3(-4.0f + i * 2.0f, 0.0f, 5.0f));
        AlienAIComponent* ai = new AlienAIComponent();
        ai->SetPatrolPoints({
            Vector3(-4.0f + i * 2.0f, 0.0f, 5.0f),
            Vector3(-4.0f + i * 2.0f, 0.0f, 7.0f),
            Vector3(-2.0f + i * 2.0f, 0.0f, 7.0f),
            Vector3(-2.0f + i * 2.0f, 0.0f, 5.0f)
        });
        alien->AddComponent(std::shared_ptr<Component>(ai));
        m_Entities.push_back(std::shared_ptr<Entity>(alien));
    }
}

void SceneManager::Shutdown() {
    m_Entities.clear();
}

void SceneManager::Update(float deltaTime) {
    // Remove dead aliens
    m_Entities.erase(
        std::remove_if(m_Entities.begin(), m_Entities.end(), [](const std::shared_ptr<Entity>& entity) {
            if (auto* alien = dynamic_cast<AlienCharacter*>(entity.get())) {
                return alien->IsDead();
            }
            return false;
        }),
        m_Entities.end()
    );

    // Update alien hit flash
    for (auto& entity : m_Entities) {
        if (auto* alien = dynamic_cast<AlienCharacter*>(entity.get())) {
            alien->UpdateFlash(deltaTime);
        }
    }

    // Check for player death
    bool playerDead = false;
    int alienCount = 0;
    float playerHealth = 0.0f;
    int playerAmmo = 0;
    for (auto& entity : m_Entities) {
        if (auto* player = dynamic_cast<HumanCharacter*>(entity.get())) {
            playerHealth = player->GetHealth();
            if (player->IsDead()) playerDead = true;
            // Find weapon for ammo
            for (auto& comp : player->GetComponents()) {
                if (auto* weapon = dynamic_cast<WeaponComponent*>(comp.get())) {
                    playerAmmo = weapon->GetAmmo();
                }
            }
        }
        if (dynamic_cast<AlienCharacter*>(entity.get())) {
            ++alienCount;
        }
    }
    UIManager::GetInstance().SetPlayerHealth(playerHealth);
    UIManager::GetInstance().SetPlayerAmmo(playerAmmo);
    if (playerDead) {
        UIManager::GetInstance().ShowMessage("Game Over! You were killed by the aliens.");
    } else if (alienCount == 0) {
        UIManager::GetInstance().ShowMessage("Victory! All aliens eliminated.");
    } else {
        UIManager::GetInstance().ClearMessage();
    }

    for (auto& entity : m_Entities) {
        for (auto& component : entity->GetComponents()) {
            component->Update(deltaTime);
        }
    }

    // Render UI
    UIManager::GetInstance().Render();
}

void SceneManager::Reset() {
    Shutdown();
    Initialize();
}

} // namespace InvasionEngine 