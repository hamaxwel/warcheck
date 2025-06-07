#pragma once

#include <vector>
#include <GL/glew.h>
#include "Math/Vector.h"

namespace InvasionEngine {

struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texCoord;
};

class Mesh {
public:
    Mesh();
    ~Mesh();

    void SetVertices(const std::vector<Vertex>& vertices);
    void SetIndices(const std::vector<unsigned int>& indices);
    void UploadToGPU();
    void Draw() const;

private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    GLuint m_VAO, m_VBO, m_EBO;
    bool m_Uploaded;
};

} // namespace InvasionEngine 