#include "MeshComponent.h"
#include <cmath>

namespace InvasionEngine {

MeshComponent::MeshComponent() {}
MeshComponent::~MeshComponent() {}

void MeshComponent::SetVertices(const std::vector<Vertex>& vertices) {
    m_Vertices = vertices;
}
void MeshComponent::SetIndices(const std::vector<unsigned int>& indices) {
    m_Indices = indices;
}
const std::vector<Vertex>& MeshComponent::GetVertices() const {
    return m_Vertices;
}
const std::vector<unsigned int>& MeshComponent::GetIndices() const {
    return m_Indices;
}

MeshComponent* MeshComponent::CreateCube(float size) {
    float h = size * 0.5f;
    std::vector<Vertex> vertices = {
        // Front
        {{-h, -h,  h}, {0, 0, 1}, {0, 0}},
        {{ h, -h,  h}, {0, 0, 1}, {1, 0}},
        {{ h,  h,  h}, {0, 0, 1}, {1, 1}},
        {{-h,  h,  h}, {0, 0, 1}, {0, 1}},
        // Back
        {{-h, -h, -h}, {0, 0, -1}, {1, 0}},
        {{ h, -h, -h}, {0, 0, -1}, {0, 0}},
        {{ h,  h, -h}, {0, 0, -1}, {0, 1}},
        {{-h,  h, -h}, {0, 0, -1}, {1, 1}},
    };
    std::vector<unsigned int> indices = {
        // Front
        0, 1, 2, 2, 3, 0,
        // Top
        3, 2, 6, 6, 7, 3,
        // Back
        7, 6, 5, 5, 4, 7,
        // Bottom
        4, 5, 1, 1, 0, 4,
        // Left
        4, 0, 3, 3, 7, 4,
        // Right
        1, 5, 6, 6, 2, 1
    };
    MeshComponent* mesh = new MeshComponent();
    mesh->SetVertices(vertices);
    mesh->SetIndices(indices);
    return mesh;
}

MeshComponent* MeshComponent::CreatePlane(float width, float depth) {
    float hw = width * 0.5f;
    float hd = depth * 0.5f;
    std::vector<Vertex> vertices = {
        {{-hw, 0, -hd}, {0, 1, 0}, {0, 0}},
        {{ hw, 0, -hd}, {0, 1, 0}, {1, 0}},
        {{ hw, 0,  hd}, {0, 1, 0}, {1, 1}},
        {{-hw, 0,  hd}, {0, 1, 0}, {0, 1}},
    };
    std::vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};
    MeshComponent* mesh = new MeshComponent();
    mesh->SetVertices(vertices);
    mesh->SetIndices(indices);
    return mesh;
}

} // namespace InvasionEngine 