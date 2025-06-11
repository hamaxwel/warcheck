#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "CameraComponent.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include <glm/gtc/type_ptr.hpp>
#include "WeaponComponent.h"
#include "AlienCharacter.h"
#include "Material.h"

namespace InvasionEngine {

Renderer::Renderer() : m_AmbientLightIntensity(0.2f), m_Shader(nullptr) {
    m_ViewMatrix = Matrix4x4::Identity();
    m_ProjectionMatrix = Matrix4x4::Identity();
    m_ModelMatrix = Matrix4x4::Identity();
    m_AmbientLightColor = Vector3(1.0f, 1.0f, 1.0f);
}

Renderer::~Renderer() {}

bool Renderer::Initialize() {
    // Load shaders
    m_Shader = std::make_unique<Shader>();
    if (!m_Shader->LoadFromFiles("Content/Shaders/basic.vert", "Content/Shaders/basic.frag")) {
        return false;
    }
    return true;
}

void Renderer::Shutdown() {
    m_Shader.reset();
}

void Renderer::BeginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndFrame() {
    // Swap buffers (handled by Window)
}

void Renderer::Clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::SetViewMatrix(const Matrix4x4& viewMatrix) {
    m_ViewMatrix = viewMatrix;
}

void Renderer::SetProjectionMatrix(const Matrix4x4& projectionMatrix) {
    m_ProjectionMatrix = projectionMatrix;
}

void Renderer::SetModelMatrix(const Matrix4x4& modelMatrix) {
    m_ModelMatrix = modelMatrix;
}

void Renderer::SetLights(const std::vector<std::shared_ptr<Light>>& lights) {
    m_Lights = lights;
}

void Renderer::SetAmbientLight(const Vector3& color, float intensity) {
    m_AmbientLightColor = color;
    m_AmbientLightIntensity = intensity;
}

void Renderer::DrawMesh(const std::shared_ptr<Mesh>& mesh, const Matrix4x4& transform) {
    if (!mesh || !m_Shader) return;

    m_Shader->Use();

    // Set matrices
    m_Shader->SetUniform("model", transform.m[0]);
    m_Shader->SetUniform("view", m_ViewMatrix.m[0]);
    m_Shader->SetUniform("projection", m_ProjectionMatrix.m[0]);

    // Set material properties if available
    if (m_CurrentMaterial) {
        m_Shader->SetUniform("material.diffuse", m_CurrentMaterial->GetDiffuseColor());
        m_Shader->SetUniform("material.specular", m_CurrentMaterial->GetSpecularColor());
        m_Shader->SetUniform("material.shininess", m_CurrentMaterial->GetShininess());
    }

    // Set light properties
    m_Shader->SetUniform("ambientLight.color", m_AmbientLightColor);
    m_Shader->SetUniform("ambientLight.intensity", m_AmbientLightIntensity);

    // Draw mesh
    mesh->Draw();

    if (m_Shader) m_Shader->Unbind();
}

void Renderer::DrawSprite(const std::string& textureName, const Vector2& position, const Vector2& size) {
    // TODO: Implement sprite rendering
}

void Renderer::DrawText(const std::string& text, const Vector2& position, float scale) {
    // TODO: Implement text rendering
}

void Renderer::SetMaterial(const std::shared_ptr<Material>& material) {
    m_CurrentMaterial = material;
}

void Renderer::RenderScene(const std::vector<std::shared_ptr<Entity>>& entities) {
    // Render all meshes
    for (const auto& entity : entities) {
        if (auto* alien = dynamic_cast<AlienCharacter*>(entity.get())) {
            // Alien: flash white if hit, otherwise green
            if (alien->IsFlashing()) {
                glColor3f(1.0f, 1.0f, 1.0f);
            } else {
                glColor3f(0.0f, 1.0f, 0.0f);
            }
        }
        // ... existing mesh rendering ...
        // Reset color after drawing
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    // ... existing shot line rendering ...
}

} // namespace InvasionEngine 