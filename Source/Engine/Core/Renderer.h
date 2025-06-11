#pragma once

#include <memory>
#include <string>
#include <vector>
#include "../Math/Matrix.h"
#include "../Math/Vector.h"

namespace InvasionEngine {

class Mesh;
class Light;
class Material;
class Shader;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Initialize();
    void Shutdown();
    void BeginFrame();
    void EndFrame();
    void Clear(float r, float g, float b, float a);

    // Matrix setup
    void SetViewMatrix(const Matrix4x4& viewMatrix);
    void SetProjectionMatrix(const Matrix4x4& projectionMatrix);
    void SetModelMatrix(const Matrix4x4& modelMatrix);

    // Lighting setup
    void SetLights(const std::vector<std::shared_ptr<Light>>& lights);
    void SetAmbientLight(const Vector3& color, float intensity);

    // Drawing methods
    void DrawMesh(const std::shared_ptr<Mesh>& mesh, const Matrix4x4& transform);
    void DrawSprite(const std::string& textureName, const Vector2& position, const Vector2& size);
    void DrawText(const std::string& text, const Vector2& position, float scale);

    // Material setup
    void SetMaterial(const std::shared_ptr<Material>& material);

private:
    // Internal renderer state
    Matrix4x4 m_ViewMatrix;
    Matrix4x4 m_ProjectionMatrix;
    Matrix4x4 m_ModelMatrix;
    std::vector<std::shared_ptr<Light>> m_Lights;
    Vector3 m_AmbientLightColor;
    float m_AmbientLightIntensity;
    std::shared_ptr<Material> m_CurrentMaterial;
    std::shared_ptr<Shader> m_Shader;
};

} // namespace InvasionEngine 