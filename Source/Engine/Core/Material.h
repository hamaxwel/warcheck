#pragma once

#include <string>
#include <memory>
#include "../Math/Vector.h"

namespace InvasionEngine {

class Texture;
class Shader;

class Material {
public:
    Material();
    ~Material();

    void SetShader(std::shared_ptr<Shader> shader);
    void SetDiffuseTexture(std::shared_ptr<Texture> texture);
    void SetNormalTexture(std::shared_ptr<Texture> texture);
    void SetSpecularTexture(std::shared_ptr<Texture> texture);
    void SetColor(const Vector4& color);
    void SetShininess(float shininess);
    void SetDiffuseColor(const Vector3& color);
    void SetSpecularColor(const Vector3& color);

    std::shared_ptr<Shader> GetShader() const { return m_Shader; }
    std::shared_ptr<Texture> GetDiffuseTexture() const { return m_DiffuseTexture; }
    std::shared_ptr<Texture> GetNormalTexture() const { return m_NormalTexture; }
    std::shared_ptr<Texture> GetSpecularTexture() const { return m_SpecularTexture; }
    const Vector4& GetColor() const { return m_Color; }
    float GetShininess() const { return m_Shininess; }
    const Vector3& GetDiffuseColor() const { return m_DiffuseColor; }
    const Vector3& GetSpecularColor() const { return m_SpecularColor; }

    void Bind() const;
    void Unbind() const;

private:
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Texture> m_DiffuseTexture;
    std::shared_ptr<Texture> m_NormalTexture;
    std::shared_ptr<Texture> m_SpecularTexture;
    Vector4 m_Color;
    float m_Shininess;
    Vector3 m_DiffuseColor;
    Vector3 m_SpecularColor;
};

} // namespace InvasionEngine 