#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "CameraComponent.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include <glm/gtc/type_ptr.hpp>
#include "WeaponComponent.h"
#include "AlienCharacter.h"

namespace InvasionEngine {

Renderer::Renderer() {}
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

void Renderer::DrawMesh(const Mesh* mesh, const CameraComponent* camera, const Vector3& position, const Quaternion& rotation, const Vector3& scale) {
    if (!mesh || !camera) return;

    m_Shader->Use();

    // Set uniforms
    Matrix4x4 model = Matrix4x4::Identity();
    model = model * Matrix4x4::Scale(scale);
    model = model * Matrix4x4::FromQuaternion(rotation);
    model = model * Matrix4x4::Translation(position);

    m_Shader->SetUniform("model", model.m[0]);
    m_Shader->SetUniform("view", camera->GetViewMatrix().m[0]);
    m_Shader->SetUniform("projection", camera->GetProjectionMatrix().m[0]);

    // Set light uniforms
    Vector3 lightDir(0.0f, -1.0f, 0.0f);
    Vector3 lightColor(1.0f, 1.0f, 1.0f);
    Vector3 objectColor(0.8f, 0.8f, 0.8f);
    m_Shader->SetUniform("lightDir", lightDir.x);
    m_Shader->SetUniform("lightColor", lightColor.x);
    m_Shader->SetUniform("objectColor", objectColor.x);

    // Draw mesh
    mesh->Draw();
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