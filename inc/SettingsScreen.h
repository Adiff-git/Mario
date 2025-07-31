#pragma once

#include "Screen.h"
#include "Button.h"
#include "raylib.h"

class SettingsScreen : public Screen {
private:
    // UI Buttons
    Button backButton;
    Button tutorialsButton;
    Button musicMinusButton;
    Button musicPlusButton;
    Button sfxMinusButton;
    Button sfxPlusButton;
    Button muteToggleButton;
    // Button pauseButton;

    // Audio settings
    int musicVolume; 
    int sfxVolume;   
    bool isMuted;

    // Pause/resume state
    // bool isPaused;

    // Show/hide tutorial
    bool showTutorial;
    Texture2D tutorialImage;
    Button closeTutorialButton;
    Texture2D closeTutorialTexture;

    void DrawTutorials();
    void DrawAudioSettings();
    // void DrawPauseButton();

    void DrawMusicVolumeControls();
    void DrawSFXVolumeControls();

public:
    SettingsScreen(ScreenController* controller);
    ~SettingsScreen();

    void Update() override;
    void Draw() override;
};
