#include "Material.h"
#include "Shader.h"
#include "Texture.h"

namespace InvasionEngine {

Material::Material() : m_Shininess(32.0f) {
    m_DiffuseColor = Vector3(0.8f, 0.8f, 0.8f);
    m_SpecularColor = Vector3(1.0f, 1.0f, 1.0f);
}

Material::~Material() {}

void Material::SetShader(std::shared_ptr<Shader> shader) {
    m_Shader = shader;
}

void Material::SetDiffuseTexture(std::shared_ptr<Texture> texture) {
    m_DiffuseTexture = texture;
}

void Material::SetNormalTexture(std::shared_ptr<Texture> texture) {
    m_NormalTexture = texture;
}

void Material::SetSpecularTexture(std::shared_ptr<Texture> texture) {
    m_SpecularTexture = texture;
}

void Material::SetColor(const Vector4& color) {
    m_Color = color;
}

void Material::SetShininess(float shininess) {
    m_Shininess = shininess;
}

void Material::SetDiffuseColor(const Vector3& color) {
    m_DiffuseColor = color;
}

void Material::SetSpecularColor(const Vector3& color) {
    m_SpecularColor = color;
}

void Material::Bind() const {
    if (m_Shader) {
        m_Shader->Use();

        // Bind textures
        if (m_DiffuseTexture) {
            m_DiffuseTexture->Bind(0);
            m_Shader->SetUniform("material.diffuseMap", 0);
        }
        if (m_NormalTexture) {
            m_NormalTexture->Bind(1);
            m_Shader->SetUniform("material.normalMap", 1);
        }
        if (m_SpecularTexture) {
            m_SpecularTexture->Bind(2);
            m_Shader->SetUniform("material.specularMap", 2);
        }

        // Set material properties
        m_Shader->SetUniform("material.diffuse", m_DiffuseColor);
        m_Shader->SetUniform("material.specular", m_SpecularColor);
        m_Shader->SetUniform("material.shininess", m_Shininess);
        m_Shader->SetUniform("material.color", m_Color);
    }
}

void Material::Unbind() const {
    if (m_DiffuseTexture) m_DiffuseTexture->Unbind();
    if (m_NormalTexture) m_NormalTexture->Unbind();
    if (m_SpecularTexture) m_SpecularTexture->Unbind();
}

} // namespace InvasionEngine 