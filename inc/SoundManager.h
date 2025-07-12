#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>

class SoundManager {
    private:
        std::unordered_map<std::string, Sound*> sounds; // Map to store sound effects
        std::unordered_map<std::string, Music*> music;
        SoundManager() = default; // Private constructor for singleton pattern
        ~SoundManager() = default;// Destructor to unload sounds
    public:
        static SoundManager& GetInstance() ;

        void PlaySound(const std::string& name);
        void PlayMusic(const std::string& name);
        void StopMusic(const std::string& name);
        void StopAllSounds();
        void Update();
};