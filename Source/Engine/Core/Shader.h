#pragma once

#include <string>
#include <unordered_map>
#include <GL/glew.h>

namespace InvasionEngine {

class Shader {
public:
    Shader();
    ~Shader();

    bool LoadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    void Use() const;
    void Unuse() const;

    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const float* matrix);

private:
    GLuint m_Program;
    std::unordered_map<std::string, GLint> m_UniformLocations;

    GLint GetUniformLocation(const std::string& name);
};

} // namespace InvasionEngine 