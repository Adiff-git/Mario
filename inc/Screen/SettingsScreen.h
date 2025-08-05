#pragma once

#include "Screen.h"
#include "Button.h"
#include "raylib.h"

class SettingsScreen : public Screen {
private:
    // UI Buttons
    Button backButton;
    Button tutorialsButton;
    Button audioSettingButton;
    Button muteToggleButton;

    // Audio settings
    int musicVolume; 
    int sfxVolume;   
    bool isMuted;
    bool showAudioSettings;

    // Show/hide tutorial
    bool showTutorial;
    Texture2D tutorialImage;
    Button closeTutorialButton;
    Texture2D closeTutorialTexture;
    
    // Background textures
    const Texture2D* backgroundTexture;
    const Texture2D* settingInterfaceTexture;

public:
    SettingsScreen(ScreenController* controller);
    ~SettingsScreen();

    void Update() override;
    void Draw() override;
};