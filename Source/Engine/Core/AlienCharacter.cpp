#include "AlienCharacter.h"
#include "Mesh.h"
#include <memory>

namespace InvasionEngine {

AlienCharacter::AlienCharacter() : m_FlashTimer(0.0f) {
    m_Health = 80.0f;
    // Use a colored sphere as a placeholder for the alien (approximate with subdivided icosahedron or just a colored cube for now)
    Mesh* mesh = new Mesh();
    // For now, use a green cube as a placeholder
    mesh->SetVertices({
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
    });
    mesh->SetIndices({
        0, 1, 2, 2, 3, 0, // Front
        3, 2, 6, 6, 7, 3, // Top
        7, 6, 5, 5, 4, 7, // Back
        4, 5, 1, 1, 0, 4, // Bottom
        4, 0, 3, 3, 7, 4, // Left
        1, 5, 6, 6, 2, 1  // Right
    });
    mesh->UploadToGPU();
    AddComponent(std::shared_ptr<Component>(mesh));
}

void AlienCharacter::TakeDamage(float amount) {
    m_Health -= amount;
    if (m_Health < 0.0f) m_Health = 0.0f;
    TriggerHitFlash();
}

void AlienCharacter::TriggerHitFlash() {
    m_FlashTimer = 0.15f; // Flash for 0.15 seconds
}

void AlienCharacter::UpdateFlash(float deltaTime) {
    if (m_FlashTimer > 0.0f) {
        m_FlashTimer -= deltaTime;
        if (m_FlashTimer < 0.0f) m_FlashTimer = 0.0f;
    }
}

} // namespace InvasionEngine 