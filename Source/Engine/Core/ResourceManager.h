#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace InvasionEngine {

class Texture;
class Mesh;
class Shader;
class Material;
class AudioClip;

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    void Initialize();
    void Shutdown();

    // TODO: Add methods for loading/unloading resources

    // Resource loading
    std::shared_ptr<Texture> LoadTexture(const std::string& path);
    std::shared_ptr<Mesh> LoadMesh(const std::string& path);
    std::shared_ptr<Shader> LoadShader(const std::string& vertexPath, const std::string& fragmentPath);
    std::shared_ptr<Material> LoadMaterial(const std::string& path);
    std::shared_ptr<AudioClip> LoadAudioClip(const std::string& path);

    // Resource management
    void UnloadTexture(const std::string& path);
    void UnloadMesh(const std::string& path);
    void UnloadShader(const std::string& name);
    void UnloadMaterial(const std::string& path);
    void UnloadAudioClip(const std::string& path);

    // Resource access
    std::shared_ptr<Texture> GetTexture(const std::string& path) const;
    std::shared_ptr<Mesh> GetMesh(const std::string& path) const;
    std::shared_ptr<Shader> GetShader(const std::string& name) const;
    std::shared_ptr<Material> GetMaterial(const std::string& path) const;
    std::shared_ptr<AudioClip> GetAudioClip(const std::string& path) const;

    // Resource monitoring
    size_t GetTextureCount() const { return m_Textures.size(); }
    size_t GetMeshCount() const { return m_Meshes.size(); }
    size_t GetShaderCount() const { return m_Shaders.size(); }
    size_t GetMaterialCount() const { return m_Materials.size(); }
    size_t GetAudioClipCount() const { return m_AudioClips.size(); }

    // Resource cleanup
    void UnloadUnusedResources();
    void UnloadAllResources();

private:
    // Resource caches
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Meshes;
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;
    std::unordered_map<std::string, std::shared_ptr<AudioClip>> m_AudioClips;

    // Resource loading helpers
    bool LoadTextureInternal(const std::string& path, std::shared_ptr<Texture>& texture);
    bool LoadMeshInternal(const std::string& path, std::shared_ptr<Mesh>& mesh);
    bool LoadShaderInternal(const std::string& vertexPath, const std::string& fragmentPath, std::shared_ptr<Shader>& shader);
    bool LoadMaterialInternal(const std::string& path, std::shared_ptr<Material>& material);
    bool LoadAudioClipInternal(const std::string& path, std::shared_ptr<AudioClip>& audioClip);
};

} // namespace InvasionEngine 