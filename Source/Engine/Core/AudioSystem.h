#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace InvasionEngine {

class AudioClip {
public:
    AudioClip(const std::string& filename);
    ~AudioClip();

    bool Load();
    void Unload();

private:
    std::string m_Filename;
    void* m_Chunk;
};

class AudioSource {
public:
    AudioSource();
    ~AudioSource();

    void SetClip(std::shared_ptr<AudioClip> clip);
    void Play();
    void Pause();
    void Stop();
    void SetVolume(float volume);
    void SetPitch(float pitch);
    void SetPosition(const glm::vec3& position);
    void SetVelocity(const glm::vec3& velocity);
    void SetMinDistance(float distance);
    void SetMaxDistance(float distance);
    bool IsPlaying() const;

private:
    std::shared_ptr<AudioClip> m_Clip;
    int m_Channel;
    float m_Volume;
    float m_Pitch;
    glm::vec3 m_Position;
    glm::vec3 m_Velocity;
    float m_MinDistance;
    float m_MaxDistance;
};

class AudioListener {
public:
    AudioListener();
    ~AudioListener();

    void SetPosition(const glm::vec3& position);
    void SetVelocity(const glm::vec3& velocity);
    void SetOrientation(const glm::vec3& forward, const glm::vec3& up);

private:
    glm::vec3 m_Position;
    glm::vec3 m_Velocity;
    glm::vec3 m_Forward;
    glm::vec3 m_Up;
};

class AudioSystem {
public:
    static bool Initialize();
    static void Shutdown();
    static void Update();

    static std::shared_ptr<AudioClip> LoadClip(const std::string& filename);
    static std::shared_ptr<AudioSource> CreateSource();
    static std::shared_ptr<AudioListener> CreateListener();

private:
    static std::vector<std::shared_ptr<AudioClip>> m_Clips;
    static std::vector<std::shared_ptr<AudioSource>> m_Sources;
    static std::shared_ptr<AudioListener> m_Listener;
};

} // namespace InvasionEngine 