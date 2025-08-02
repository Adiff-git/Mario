#include "SoundManager.h"
#include "ResrcManager.h"
#include <iostream>
using namespace std;

SoundManager& SoundManager::GetInstance() {
    static SoundManager instance; // Create a static instance of SoundManager
    return instance;
}

void SoundManager::PlaySound(const std::string& name) {
    ::PlaySound(ResrcManager::GetInstance().getSound(name));
    sounds[name] = &ResrcManager::GetInstance().getSound(name);
}

void SoundManager::PlayMusic(const std::string& name) {
    Music& musicRef = ResrcManager::GetInstance().getMusic(name);
    std::cout << "Playing music: " << name << std::endl; // Debug log
    ::PlayMusicStream(musicRef);
    music[name] = &musicRef;
    
    // Set volume if it exists in the map, otherwise use 1.0f
    float volume = 1.0f;
    if (musicVolumes.find(name) != musicVolumes.end()) {
        volume = musicVolumes[name];
    }
    SetMusicVolume(musicRef, volume * masterVolume);
    
    std::cout << "Music playing status: " << IsMusicStreamPlaying(musicRef) << std::endl; // Debug log
}

void SoundManager::StopMusic(const std::string& name) {
    if (music.find(name) != music.end()) {
        StopMusicStream(*music[name]);
    }
}

// ...existing code...
void SoundManager::StopAllSounds() {
    for (const auto& sound : sounds) {
        StopSound(*sound.second);
    }
    for (const auto& musicItem : music) {
        StopMusicStream(*musicItem.second);
    }
    // Clear the maps after stopping
    sounds.clear();
    music.clear();
}
// ...existing code...

void SoundManager::SetMasterVol(float volume) {
    masterVolume = std::max(0.0f, std::min(1.0f, volume)); // Clamp between 0-1
    SetMasterVolume(masterVolume);
    std::cout << "Master volume set to: " << masterVolume << std::endl;
    
    // Update all currently playing music volumes
    for (const auto& musicItem : music) {
        float musicVol = 1.0f;
        if (musicVolumes.find(musicItem.first) != musicVolumes.end()) {
            musicVol = musicVolumes[musicItem.first];
        }
        SetMusicVolume(*musicItem.second, musicVol * masterVolume);
    }
}

void SoundManager::SetMusicVol(const std::string& name, float volume) {
    volume = std::max(0.0f, std::min(1.0f, volume)); // Clamp between 0-1
    musicVolumes[name] = volume; // Store in separate volume map
    
    if (music.find(name) != music.end()) {
        SetMusicVolume(*music[name], volume * masterVolume);
        std::cout << "Music '" << name << "' volume set to: " << volume << std::endl;
    }
}

void SoundManager::SetSoundVol(const std::string& name, float volume) {
    volume = std::max(0.0f, std::min(1.0f, volume)); // Clamp between 0-1
    soundVolumes[name] = volume; // Store in separate volume map
    
    if (sounds.find(name) != sounds.end()) {
        SetSoundVolume(*sounds[name], volume * masterVolume);
        std::cout << "Sound '" << name << "' volume set to: " << volume << std::endl;
    }
}

float SoundManager::GetMasterVol() {
    return masterVolume;
}

float SoundManager::GetMusicVol(const std::string& name) {
    if (musicVolumes.find(name) != musicVolumes.end()) {
        return musicVolumes[name];
    }
    return 1.0f; // Default volume
}

void SoundManager::Update() {
    for (const auto& sound : sounds) {
        if (!IsSoundPlaying(*sound.second)) {
            StopSound(*sound.second);
        }
    }
    for (const auto& musicItem : music) {
        // Update music stream
        UpdateMusicStream(*musicItem.second);
        
        // Apply volume from volume map if exists, otherwise use 1.0f
        float volume = 1.0f;
        if (musicVolumes.find(musicItem.first) != musicVolumes.end()) {
            volume = musicVolumes[musicItem.first];
        }
        SetMusicVolume(*musicItem.second, volume * masterVolume);
        
        if (!IsMusicStreamPlaying(*musicItem.second)) {
            StopMusicStream(*musicItem.second);
        }
    }
}