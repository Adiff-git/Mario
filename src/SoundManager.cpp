#include "SoundManager.h"
#include "ResrcManager.h"

SoundManager& SoundManager::GetInstance() {
    static SoundManager instance; // Create a static instance of SoundManager
    return instance;
}

void SoundManager::PlaySound(const std::string& name) {
    ::PlaySound(ResrcManager::GetInstance().getSound(name));
    sounds[name] = &ResrcManager::GetInstance().getSound(name);
}

void SoundManager::PlayMusic(const std::string& name) {
    ::PlayMusicStream(ResrcManager::GetInstance().getMusic(name));
    music[name] = &ResrcManager::GetInstance().getMusic(name);
}

void SoundManager::StopMusic(const std::string& name) {
    if (music.find(name) != music.end()) {
        StopMusicStream(*music[name]);
    }
}

void SoundManager::StopAllSounds() {
    for (const auto& sound : sounds) {
        StopSound(*sound.second);
    }
    for (const auto& musicItem : music) {
        StopMusicStream(*musicItem.second);
    }
}

void SoundManager::Update() {
    for (const auto& sound : sounds) {
        if (!IsSoundPlaying(*sound.second)) {
            StopSound(*sound.second);
        }
    }
    for (const auto& musicItem : music) {
        if (!IsMusicStreamPlaying(*musicItem.second)) {
            StopMusicStream(*musicItem.second);
        }
    }
}