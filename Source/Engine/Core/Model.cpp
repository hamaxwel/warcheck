#include "Model.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace InvasionEngine {

Model::Model(const std::string& filename) {
    LoadOBJ(filename);
}

void Model::LoadOBJ(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error loading OBJ file: " << filename << std::endl;
        return;
    }

    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector2> texCoords;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        if (prefix == "v") {
            Vector3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        } else if (prefix == "vt") {
            Vector2 tex;
            iss >> tex.x >> tex.y;
            texCoords.push_back(tex);
        } else if (prefix == "vn") {
            Vector3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        } else if (prefix == "f") {
            unsigned int vIdx[3], tIdx[3], nIdx[3];
            char slash;
            for (int i = 0; i < 3; ++i) {
                iss >> vIdx[i] >> slash >> tIdx[i] >> slash >> nIdx[i];
                Vertex vert;
                vert.position = positions[vIdx[i] - 1];
                vert.texCoord = texCoords.size() > 0 ? texCoords[tIdx[i] - 1] : Vector2(0,0);
                vert.normal = normals.size() > 0 ? normals[nIdx[i] - 1] : Vector3(0,0,1);
                vertices.push_back(vert);
                indices.push_back(static_cast<unsigned int>(vertices.size() - 1));
            }
        }
    }

    auto mesh = std::make_shared<Mesh>();
    mesh->SetVertices(vertices);
    mesh->SetIndices(indices);
    mesh->UploadToGPU();
    m_Meshes.push_back(mesh);
    m_Materials.push_back(std::make_shared<Material>()); // Default material
}

void Model::Draw() const {
    for (const auto& mesh : m_Meshes) {
        mesh->Draw();
    }
}

} // namespace InvasionEngine 