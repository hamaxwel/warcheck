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

    std::shared_ptr<Shader> GetShader() const { return m_Shader; }
    std::shared_ptr<Texture> GetDiffuseTexture() const { return m_DiffuseTexture; }
    std::shared_ptr<Texture> GetNormalTexture() const { return m_NormalTexture; }
    std::shared_ptr<Texture> GetSpecularTexture() const { return m_SpecularTexture; }
    const Vector4& GetColor() const { return m_Color; }
    float GetShininess() const { return m_Shininess; }

    void Bind() const;
    void Unbind() const;

private:
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Texture> m_DiffuseTexture;
    std::shared_ptr<Texture> m_NormalTexture;
    std::shared_ptr<Texture> m_SpecularTexture;
    Vector4 m_Color;
    float m_Shininess;
};

} // namespace InvasionEngine 