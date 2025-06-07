#include "AudioSystem.h"
#include <stb_vorbis.h>
#include <iostream>

namespace InvasionEngine {

AudioClip::AudioClip(const std::string& filename) : m_Buffer(0) {
    alGenBuffers(1, &m_Buffer);
    
    // Load audio file (supporting .ogg files via stb_vorbis)
    int channels, sampleRate;
    short* data;
    int samples = stb_vorbis_decode_filename(filename.c_str(), &channels, &sampleRate, &data);
    
    if (samples > 0) {
        ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        alBufferData(m_Buffer, format, data, samples * channels * sizeof(short), sampleRate);
        free(data);
    } else {
        std::cerr << "Failed to load audio file: " << filename << std::endl;
        alDeleteBuffers(1, &m_Buffer);
        m_Buffer = 0;
    }
}

AudioClip::~AudioClip() {
    if (m_Buffer != 0) {
        alDeleteBuffers(1, &m_Buffer);
    }
}

AudioSource::AudioSource() : m_Source(0), m_Volume(1.0f), m_Pitch(1.0f),
    m_MinDistance(1.0f), m_MaxDistance(100.0f) {
    alGenSources(1, &m_Source);
    alSourcef(m_Source, AL_GAIN, m_Volume);
    alSourcef(m_Source, AL_PITCH, m_Pitch);
    alSourcef(m_Source, AL_MIN_GAIN, 0.0f);
    alSourcef(m_Source, AL_MAX_GAIN, 1.0f);
    alSourcef(m_Source, AL_REFERENCE_DISTANCE, m_MinDistance);
    alSourcef(m_Source, AL_MAX_DISTANCE, m_MaxDistance);
}

AudioSource::~AudioSource() {
    if (m_Source != 0) {
        alDeleteSources(1, &m_Source);
    }
}

void AudioSource::SetClip(const std::shared_ptr<AudioClip>& clip) {
    m_Clip = clip;
    if (clip && clip->IsValid()) {
        alSourcei(m_Source, AL_BUFFER, clip->GetBuffer());
    }
}

void AudioSource::Play() {
    if (m_Clip && m_Clip->IsValid()) {
        alSourcePlay(m_Source);
    }
}

void AudioSource::Pause() {
    alSourcePause(m_Source);
}

void AudioSource::Stop() {
    alSourceStop(m_Source);
}

void AudioSource::SetLooping(bool loop) {
    alSourcei(m_Source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void AudioSource::SetVolume(float volume) {
    m_Volume = volume;
    alSourcef(m_Source, AL_GAIN, volume);
}

void AudioSource::SetPitch(float pitch) {
    m_Pitch = pitch;
    alSourcef(m_Source, AL_PITCH, pitch);
}

void AudioSource::SetPosition(const Vector3& position) {
    m_Position = position;
    alSource3f(m_Source, AL_POSITION, position.x, position.y, position.z);
}

void AudioSource::SetVelocity(const Vector3& velocity) {
    m_Velocity = velocity;
    alSource3f(m_Source, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void AudioSource::SetMinDistance(float distance) {
    m_MinDistance = distance;
    alSourcef(m_Source, AL_REFERENCE_DISTANCE, distance);
}

void AudioSource::SetMaxDistance(float distance) {
    m_MaxDistance = distance;
    alSourcef(m_Source, AL_MAX_DISTANCE, distance);
}

bool AudioSource::IsPlaying() const {
    ALint state;
    alGetSourcei(m_Source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

AudioListener::AudioListener() : m_Forward(0.0f, 0.0f, 1.0f), m_Up(0.0f, 1.0f, 0.0f) {
    SetPosition(Vector3(0.0f));
    SetVelocity(Vector3(0.0f));
    SetOrientation(m_Forward, m_Up);
}

AudioListener::~AudioListener() {}

void AudioListener::SetPosition(const Vector3& position) {
    m_Position = position;
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void AudioListener::SetVelocity(const Vector3& velocity) {
    m_Velocity = velocity;
    alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void AudioListener::SetOrientation(const Vector3& forward, const Vector3& up) {
    m_Forward = forward;
    m_Up = up;
    float orientation[] = {
        forward.x, forward.y, forward.z,
        up.x, up.y, up.z
    };
    alListenerfv(AL_ORIENTATION, orientation);
}

AudioSystem& AudioSystem::GetInstance() {
    static AudioSystem instance;
    return instance;
}

AudioSystem::AudioSystem() : m_Device(nullptr), m_Context(nullptr),
    m_MasterVolume(1.0f), m_3DAudioEnabled(true), m_ReverbEnabled(false) {
    m_ReverbProps = {
        1.0f, 1.0f, 0.32f, 0.89f, 1.49f, 0.83f,
        0.05f, 0.007f, 1.26f, 0.011f, 0.994f, 0.0f
    };
}

AudioSystem::~AudioSystem() {
    Shutdown();
}

bool AudioSystem::Initialize() {
    m_Device = alcOpenDevice(nullptr);
    if (!m_Device) {
        std::cerr << "Failed to open audio device" << std::endl;
        return false;
    }

    m_Context = alcCreateContext(m_Device, nullptr);
    if (!m_Context) {
        std::cerr << "Failed to create audio context" << std::endl;
        alcCloseDevice(m_Device);
        return false;
    }

    alcMakeContextCurrent(m_Context);
    return true;
}

void AudioSystem::Shutdown() {
    m_Sources.clear();
    m_Clips.clear();

    if (m_Context) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(m_Context);
        m_Context = nullptr;
    }

    if (m_Device) {
        alcCloseDevice(m_Device);
        m_Device = nullptr;
    }
}

void AudioSystem::Update() {
    // Update listener position/velocity based on camera
    // This would typically be called from the game loop
}

std::shared_ptr<AudioSource> AudioSystem::CreateSource() {
    auto source = std::make_shared<AudioSource>();
    m_Sources.push_back(source);
    return source;
}

void AudioSystem::DestroySource(std::shared_ptr<AudioSource> source) {
    auto it = std::find(m_Sources.begin(), m_Sources.end(), source);
    if (it != m_Sources.end()) {
        m_Sources.erase(it);
    }
}

void AudioSystem::SetMasterVolume(float volume) {
    m_MasterVolume = volume;
    alListenerf(AL_GAIN, volume);
}

void AudioSystem::Set3DAudioEnabled(bool enabled) {
    m_3DAudioEnabled = enabled;
    // Implementation would depend on specific 3D audio features
}

void AudioSystem::SetReverbEnabled(bool enabled) {
    m_ReverbEnabled = enabled;
    // Implementation would depend on specific reverb features
}

void AudioSystem::SetReverbProperties(const ReverbProperties& props) {
    m_ReverbProps = props;
    // Implementation would depend on specific reverb features
}

std::shared_ptr<AudioClip> AudioSystem::LoadAudioClip(const std::string& filename) {
    auto it = m_Clips.find(filename);
    if (it != m_Clips.end()) {
        return it->second;
    }

    auto clip = std::make_shared<AudioClip>(filename);
    if (clip->IsValid()) {
        m_Clips[filename] = clip;
        return clip;
    }
    return nullptr;
}

} // namespace InvasionEngine 