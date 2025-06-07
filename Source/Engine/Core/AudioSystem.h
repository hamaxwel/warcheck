#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include "Math/Vector.h"

namespace InvasionEngine {

class AudioClip {
public:
    AudioClip(const std::string& filename);
    ~AudioClip();

    ALuint GetBuffer() const { return m_Buffer; }
    bool IsValid() const { return m_Buffer != 0; }

private:
    ALuint m_Buffer;
};

class AudioSource {
public:
    AudioSource();
    ~AudioSource();

    void SetClip(const std::shared_ptr<AudioClip>& clip);
    void Play();
    void Pause();
    void Stop();
    void SetLooping(bool loop);
    void SetVolume(float volume);
    void SetPitch(float pitch);
    void SetPosition(const Vector3& position);
    void SetVelocity(const Vector3& velocity);
    void SetMinDistance(float distance);
    void SetMaxDistance(float distance);

    bool IsPlaying() const;
    float GetVolume() const { return m_Volume; }
    float GetPitch() const { return m_Pitch; }
    Vector3 GetPosition() const { return m_Position; }

private:
    ALuint m_Source;
    std::shared_ptr<AudioClip> m_Clip;
    float m_Volume;
    float m_Pitch;
    Vector3 m_Position;
    Vector3 m_Velocity;
    float m_MinDistance;
    float m_MaxDistance;
};

class AudioListener {
public:
    AudioListener();
    ~AudioListener();

    void SetPosition(const Vector3& position);
    void SetVelocity(const Vector3& velocity);
    void SetOrientation(const Vector3& forward, const Vector3& up);

    Vector3 GetPosition() const { return m_Position; }
    Vector3 GetVelocity() const { return m_Velocity; }

private:
    Vector3 m_Position;
    Vector3 m_Velocity;
    Vector3 m_Forward;
    Vector3 m_Up;
};

class AudioSystem {
public:
    static AudioSystem& GetInstance();

    bool Initialize();
    void Shutdown();
    void Update();

    // Audio source management
    std::shared_ptr<AudioSource> CreateSource();
    void DestroySource(std::shared_ptr<AudioSource> source);

    // Listener management
    AudioListener& GetListener() { return m_Listener; }

    // Global audio settings
    void SetMasterVolume(float volume);
    void Set3DAudioEnabled(bool enabled);
    void SetReverbEnabled(bool enabled);
    void SetReverbProperties(const struct ReverbProperties& props);

    // Audio loading
    std::shared_ptr<AudioClip> LoadAudioClip(const std::string& filename);

private:
    AudioSystem();
    ~AudioSystem();

    ALCdevice* m_Device;
    ALCcontext* m_Context;
    std::vector<std::shared_ptr<AudioSource>> m_Sources;
    AudioListener m_Listener;
    std::unordered_map<std::string, std::shared_ptr<AudioClip>> m_Clips;
    float m_MasterVolume;
    bool m_3DAudioEnabled;
    bool m_ReverbEnabled;
    struct ReverbProperties m_ReverbProps;
};

struct ReverbProperties {
    float density;
    float diffusion;
    float gain;
    float gainHF;
    float decayTime;
    float decayHFRatio;
    float reflectionsGain;
    float reflectionsDelay;
    float lateReverbGain;
    float lateReverbDelay;
    float airAbsorptionGainHF;
    float roomRolloffFactor;
};

} // namespace InvasionEngine 