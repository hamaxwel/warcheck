#include "Mesh.h"

namespace InvasionEngine {

Mesh::Mesh() : m_VAO(0), m_VBO(0), m_EBO(0), m_Uploaded(false) {}
Mesh::~Mesh() {
    if (m_Uploaded) {
        glDeleteVertexArrays(1, &m_VAO);
        glDeleteBuffers(1, &m_VBO);
        glDeleteBuffers(1, &m_EBO);
    }
}

void Mesh::SetVertices(const std::vector<Vertex>& vertices) {
    m_Vertices = vertices;
}

void Mesh::SetIndices(const std::vector<unsigned int>& indices) {
    m_Indices = indices;
}

void Mesh::UploadToGPU() {
    if (m_Uploaded) return;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
    glEnableVertexAttribArray(1);

    // TexCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    m_Uploaded = true;
}

void Mesh::Draw() const {
    if (!m_Uploaded) return;
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // namespace InvasionEngine 