#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>

class SoundManager {
private:
    std::unordered_map<std::string, Sound*> sounds; // Map to store sound effects
    std::unordered_map<std::string, Music*> music;  // Map to store music streams
    std::unordered_map<std::string, float> musicVolumes; // Separate map for music volumes
    std::unordered_map<std::string, float> soundVolumes; // Separate map for sound volumes
    
    float masterVolume = 1.0f;
    bool isMuted = false;
    
    SoundManager() = default; // Private constructor for singleton pattern
    ~SoundManager() = default; // Destructor

public:
    static SoundManager& GetInstance();

    void PlaySound(const std::string& name);
    void PlayMusic(const std::string& name);
    void StopMusic(const std::string& name);
    void StopAllSounds();
    void SetAllMusicVol(float volume);
    void SetAllSoundVol(float volume);
    void Update();
    
    // Volume control methods - now public
    void SetMasterVol(float volume);
    void SetMusicVol(const std::string& name, float volume);
    void SetSoundVol(const std::string& name, float volume);
    
    float GetMasterVol();
    float GetMusicVol(const std::string& name);
    float GetSoundVol(const std::string& name); // Thêm hàm GetSoundVol
    void SetMuted(bool muted);
    bool IsMuted() const { return isMuted; }
};