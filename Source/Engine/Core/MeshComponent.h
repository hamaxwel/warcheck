#pragma once

#include "Component.h"
#include <string>
#include <vector>

namespace InvasionEngine {

struct Vertex {
    float position[3];
    float normal[3];
    float texcoord[2];
};

// Forward declaration only if needed
class MeshComponent;

class MeshComponent : public Component {
public:
    MeshComponent();
    virtual ~MeshComponent();

    void SetVertices(const std::vector<Vertex>& vertices);
    void SetIndices(const std::vector<unsigned int>& indices);
    const std::vector<Vertex>& GetVertices() const;
    const std::vector<unsigned int>& GetIndices() const;

    // Utility: Generate cube or plane
    static MeshComponent* CreateCube(float size = 1.0f);
    static MeshComponent* CreatePlane(float width = 10.0f, float depth = 10.0f);

private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
};

} // namespace InvasionEngine 