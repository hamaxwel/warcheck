#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Mesh.h"
#include "Material.h"

namespace InvasionEngine {

class Model {
public:
    Model(const std::string& filename); // Loads an OBJ file
    ~Model() = default;

    void Draw() const;
    const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return m_Meshes; }
    const std::vector<std::shared_ptr<Material>>& GetMaterials() const { return m_Materials; }

private:
    void LoadOBJ(const std::string& filename);
    std::vector<std::shared_ptr<Mesh>> m_Meshes;
    std::vector<std::shared_ptr<Material>> m_Materials;
};

} // namespace InvasionEngine 