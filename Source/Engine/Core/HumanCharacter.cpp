#include "HumanCharacter.h"
#include "Mesh.h"
#include <memory>

namespace InvasionEngine {

HumanCharacter::HumanCharacter() {
    m_Health = 100.0f;
    // Use a colored cube as a placeholder for the human
    Mesh* mesh = new Mesh();
    mesh->SetVertices({
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
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

void HumanCharacter::TakeDamage(float amount) {
    m_Health -= amount;
    if (m_Health < 0.0f) m_Health = 0.0f;
}

} // namespace InvasionEngine 