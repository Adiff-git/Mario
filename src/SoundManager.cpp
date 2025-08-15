#include "SoundManager.h"
#include "ResrcManager.h"
#include <iostream>

using namespace std;

SoundManager& SoundManager::GetInstance() {
    static SoundManager instance;
    return instance;
} 

void SoundManager::PlaySound(const std::string& name) {
    try {
        if (isMuted) {
            std::cout << "[SoundManager] Sound '" << name << "' blocked due to mute" << std::endl;
            return;
        }
        Sound& sound = ResrcManager::GetInstance().getSound(name);
        sounds[name] = &sound;
        float volume = soundVolumes.find(name) != soundVolumes.end() ? soundVolumes[name] : 1.0f;
        SetSoundVolume(*sounds[name], volume * masterVolume);
        ::PlaySound(sound);
        std::cout << "[SoundManager] Playing sound '" << name << "', volume: " << (volume * masterVolume) << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[SoundManager] Error playing sound '" << name << "': " << e.what() << std::endl;
    }
}

void SoundManager::PlayMusic(const std::string& name) {
    try {
        Music& musicRef = ResrcManager::GetInstance().getMusic(name);
        std::cout << "[SoundManager] Playing music: " << name << ", muted: " << isMuted << std::endl;
        ::PlayMusicStream(musicRef);
        music[name] = &musicRef;
        float volume = musicVolumes.find(name) != musicVolumes.end() ? musicVolumes[name] : 1.0f;
        SetMusicVolume(*music[name], isMuted ? 0.0f : volume * masterVolume);
        if (isMuted) {
            PauseMusicStream(*music[name]);
        }
    } catch (const std::exception& e) {
        std::cout << "[SoundManager] Error playing music '" << name << "': " << e.what() << std::endl;
    }
}

void SoundManager::StopMusic(const std::string& name) {
    if (music.find(name) != music.end()) {
        StopMusicStream(*music[name]);
        std::cout << "[SoundManager] Stopped music: " << name << std::endl;
    }
}

void SoundManager::StopAllSounds() {
    for (const auto& sound : sounds) {
        StopSound(*sound.second);
    }
    for (const auto& musicItem : music) {
        StopMusicStream(*musicItem.second);
    }
    sounds.clear();
    std::cout << "[SoundManager] All sounds stopped" << std::endl;
}

void SoundManager::SetMasterVol(float volume) {
    masterVolume = std::max(0.0f, std::min(1.0f, volume));
    SetMasterVolume(masterVolume);
    std::cout << "[SoundManager] Master volume set to: " << masterVolume << std::endl;
    for (const auto& musicItem : music) {
        float musicVol = musicVolumes.find(musicItem.first) != musicVolumes.end() ? musicVolumes[musicItem.first] : 1.0f;
        SetMusicVolume(*musicItem.second, isMuted ? 0.0f : musicVol * masterVolume);
        if (isMuted) {
            PauseMusicStream(*musicItem.second);
        } else if (!IsMusicStreamPlaying(*musicItem.second)) {
            ResumeMusicStream(*musicItem.second);
        }
    }
    for (const auto& sound : sounds) {
        float soundVol = soundVolumes.find(sound.first) != soundVolumes.end() ? soundVolumes[sound.first] : 1.0f;
        SetSoundVolume(*sound.second, isMuted ? 0.0f : soundVol * masterVolume);
        if (isMuted) {
            StopSound(*sound.second);
        }
    }
}

void SoundManager::SetMusicVol(const std::string& name, float volume) {
    volume = std::max(0.0f, std::min(1.0f, volume));
    musicVolumes[name] = volume;
    if (music.find(name) != music.end()) {
        float finalVolume = isMuted ? 0.0f : volume * masterVolume;
        SetMusicVolume(*music[name], finalVolume);
        if (isMuted) {
            PauseMusicStream(*music[name]);
        } else if (!IsMusicStreamPlaying(*music[name])) {
            ResumeMusicStream(*music[name]);
        }
        std::cout << "[SoundManager] Music '" << name << "' volume set to: " << finalVolume << ", muted: " << isMuted << std::endl;
    }
}

void SoundManager::SetAllMusicVol(float volume) {
    volume = std::max(0.0f, std::min(1.0f, volume));
    for (auto& musicItem : musicVolumes) {
        musicItem.second = volume;
        if (music.find(musicItem.first) != music.end()) {
            float finalVolume = isMuted ? 0.0f : volume * masterVolume;
            SetMusicVolume(*music[musicItem.first], finalVolume);
            if (isMuted) {
                PauseMusicStream(*music[musicItem.first]);
            } else if (!IsMusicStreamPlaying(*music[musicItem.first])) {
                ResumeMusicStream(*music[musicItem.first]);
            }
            std::cout << "[SoundManager] Music '" << musicItem.first << "' volume set to: " << finalVolume << ", muted: " << isMuted << std::endl;
        }
    }
}

void SoundManager::SetSoundVol(const std::string& name, float volume) {
    volume = std::max(0.0f, std::min(1.0f, volume));
    soundVolumes[name] = volume;
    if (sounds.find(name) != sounds.end()) {
        float finalVolume = isMuted ? 0.0f : volume * masterVolume;
        SetSoundVolume(*sounds[name], finalVolume);
        if (isMuted) {
            StopSound(*sounds[name]);
        }
        std::cout << "[SoundManager] Sound '" << name << "' volume set to: " << finalVolume << ", muted: " << isMuted << std::endl;
    }
}

float SoundManager::GetMasterVol() {
    return masterVolume;
}

float SoundManager::GetMusicVol(const std::string& name) {
    if (musicVolumes.find(name) != musicVolumes.end()) {
        return musicVolumes[name];
    }
    return 1.0f;
}

float SoundManager::GetSoundVol(const std::string& name) {
    if (soundVolumes.find(name) != soundVolumes.end()) {
        return soundVolumes[name];
    }
    return 1.0f;
}

void SoundManager::SetMuted(bool muted) {
    isMuted = muted;
    for (const auto& musicItem : music) {
        float volume = musicVolumes.find(musicItem.first) != musicVolumes.end() ? musicVolumes[musicItem.first] : 1.0f;
        SetMusicVolume(*musicItem.second, isMuted ? 0.0f : volume * masterVolume);
        if (isMuted) {
            PauseMusicStream(*musicItem.second);
        } else if (!IsMusicStreamPlaying(*musicItem.second)) {
            ResumeMusicStream(*musicItem.second);
        }
    }
    for (const auto& sound : sounds) {
        float volume = soundVolumes.find(sound.first) != soundVolumes.end() ? soundVolumes[sound.first] : 1.0f;
        SetSoundVolume(*sound.second, isMuted ? 0.0f : volume * masterVolume);
        if (isMuted) {
            StopSound(*sound.second);
        }
    }
    sounds.clear();
    std::cout << "[SoundManager] Muted: " << isMuted << std::endl;
}

void SoundManager::Update() {
    for (auto it = music.begin(); it != music.end(); ++it) {
        UpdateMusicStream(*it->second);
        float volume = musicVolumes.find(it->first) != musicVolumes.end() ? musicVolumes[it->first] : 1.0f;
        SetMusicVolume(*it->second, isMuted ? 0.0f : volume * masterVolume);
        if (isMuted && IsMusicStreamPlaying(*it->second)) {
            PauseMusicStream(*it->second);
        } else if (!isMuted && !IsMusicStreamPlaying(*it->second)) {
            ResumeMusicStream(*it->second);
        }
    }
    for (auto it = sounds.begin(); it != sounds.end();) {
        if (!IsSoundPlaying(*it->second)) {
            it = sounds.erase(it);
        } else {
            float volume = soundVolumes.find(it->first) != soundVolumes.end() ? soundVolumes[it->first] : 1.0f;
            SetSoundVolume(*it->second, isMuted ? 0.0f : volume * masterVolume);
            if (isMuted) {
                StopSound(*it->second);
                it = sounds.erase(it);
            } else {
                ++it;
            }
        }
    }
}