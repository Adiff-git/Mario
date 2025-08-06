#include "../inc/Screen/SettingsScreen.h"
#include "../inc/Screen/MenuScreen.h"
#include "../inc/ResrcManager.h"
#include "../inc/SoundManager.h"
#include <raylib.h>
#include <iostream>

SettingsScreen::SettingsScreen(ScreenController* screenController)
    : Screen(screenController),
      tutorialsButton(Vector2{(float)GetScreenWidth()/2 - 120, 310}, Vector2{240, 60}),
      audioSettingButton(Vector2{(float)GetScreenWidth()/2 - 120, 390}, Vector2{240, 60}),
      muteToggleButton(Vector2{(float)GetScreenWidth()/2 - 120, 600}, Vector2{240, 60}),
      backButton(Vector2{50, 50}, Vector2{80, 80}),
      musicVolume(50),
      sfxVolume(50),
      isMuted(SoundManager::GetInstance().IsMuted()),
      showAudioSettings(false),
      showTutorial(false)
{
    tutorialImage = LoadTexture("../resources/Menu/Tutorials.png"); 
    closeTutorialButton = Button(Vector2{100, 100}, Vector2{40, 40}, "X");
    closeTutorialTexture = LoadTexture("resources/Menu/CloseX.png");
    
    backgroundTexture = &ResrcManager::GetInstance().getTexture("BACKGROUND_10");
    settingInterfaceTexture = &ResrcManager::GetInstance().getTexture("SETTING INTERFACE");
    tutorialsButton.SetTexture(ResrcManager::GetInstance().getTexture("TUTORIALS"));
    audioSettingButton.SetTexture(ResrcManager::GetInstance().getTexture("AUDIO SETTING"));
    muteToggleButton.SetTexture(isMuted ? 
        ResrcManager::GetInstance().getTexture("MUTE ALL OFF") : 
        ResrcManager::GetInstance().getTexture("MUTE ALL ON"));
    backButton.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
    
    musicVolume = 50;
    sfxVolume = 50;
    SoundManager::GetInstance().SetMusicVol("MENU", musicVolume / 100.0f);
    SoundManager::GetInstance().SetMusicVol("GAMEWORLD_1", musicVolume / 100.0f);
    SoundManager::GetInstance().SetSoundVol("COIN_COLLECTION", sfxVolume / 100.0f);
    SoundManager::GetInstance().SetSoundVol("POWER_UP_APPEARS", sfxVolume / 100.0f);
    SoundManager::GetInstance().SetSoundVol("ENEMY_DEATH", sfxVolume / 100.0f);
    SoundManager::GetInstance().SetSoundVol("MARIO_JUMP", sfxVolume / 100.0f);
    SoundManager::GetInstance().SetSoundVol("BUTTON_CLICK", sfxVolume / 100.0f);
    std::cout << "[SettingsScreen] Initialized with isMuted: " << isMuted 
              << ", musicVolume: " << musicVolume 
              << ", sfxVolume: " << sfxVolume << std::endl;
}

SettingsScreen::~SettingsScreen() {
    UnloadTexture(tutorialImage);
    UnloadTexture(closeTutorialTexture);
}

void SettingsScreen::Update() {
    if (showTutorial) {
        closeTutorialButton.Update();
        if (closeTutorialButton.IsPressed()) {
            showTutorial = false;
        }
    } else {
        int screenW = GetScreenWidth();
        int centerGap = (450 + 600) / 2;
        int sliderStartY = centerGap - 45;
        int sliderW = 250;
        int sliderX = screenW/2 - sliderW/2;
        int musicSliderY = sliderStartY;
        
        Rectangle musicSliderRect = { (float)sliderX - 10, (float)musicSliderY - 15, (float)sliderW + 20, 50 };
        if (!SoundManager::GetInstance().IsMuted() && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), musicSliderRect)) {
            float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
            percent = std::max(0.0f, std::min(1.0f, percent));
            musicVolume = (int)(percent * 100);
            // Cập nhật volume cho tất cả music
            SoundManager::GetInstance().SetMusicVol("MENU", musicVolume / 100.0f);
            SoundManager::GetInstance().SetMusicVol("GAMEWORLD_1", musicVolume / 100.0f);
            std::cout << "[SettingsScreen] Music volume set to: " << musicVolume << std::endl;
        }

        int sfxSliderY = sliderStartY + 70;
        Rectangle sfxSliderRect = { (float)sliderX - 10, (float)sfxSliderY - 15, (float)sliderW + 20, 50 };
        if (!SoundManager::GetInstance().IsMuted() && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), sfxSliderRect)) {
            float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
            percent = std::max(0.0f, std::min(1.0f, percent));
            sfxVolume = (int)(percent * 100);
            SoundManager::GetInstance().SetSoundVol("COIN_COLLECTION", sfxVolume / 100.0f);
            SoundManager::GetInstance().SetSoundVol("POWER_UP_APPEARS", sfxVolume / 100.0f);
            SoundManager::GetInstance().SetSoundVol("ENEMY_DEATH", sfxVolume / 100.0f);
            SoundManager::GetInstance().SetSoundVol("MARIO_JUMP", sfxVolume / 100.0f);
            SoundManager::GetInstance().SetSoundVol("BUTTON_CLICK", sfxVolume / 100.0f);
            std::cout << "[SettingsScreen] SFX volume set to: " << sfxVolume << std::endl;
        }

        tutorialsButton.Update();
        audioSettingButton.Update();
        muteToggleButton.Update();
        backButton.Update(); 
        
        if (tutorialsButton.IsPressed()) {
            if (!SoundManager::GetInstance().IsMuted()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            showTutorial = !showTutorial;
        }
        
        if (audioSettingButton.IsPressed()) {
            if (!SoundManager::GetInstance().IsMuted()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            showAudioSettings = !showAudioSettings;
        }
        
        if (muteToggleButton.IsPressed()) {
            if (!SoundManager::GetInstance().IsMuted()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            isMuted = !isMuted;
            SoundManager::GetInstance().SetMuted(isMuted); 
            muteToggleButton.SetTexture(isMuted ? 
                ResrcManager::GetInstance().getTexture("MUTE ALL OFF") : 
                ResrcManager::GetInstance().getTexture("MUTE ALL ON"));
            
            if (isMuted) {
                // Set both sliders to 0 when muted
                musicVolume = 0;
                sfxVolume = 0;
                // Update SoundManager to set all music and SFX volumes to 0
                SoundManager::GetInstance().SetMusicVol("MENU", 0.0f);
                SoundManager::GetInstance().SetMusicVol("GAMEWORLD_1", 0.0f);
                SoundManager::GetInstance().SetSoundVol("COIN_COLLECTION", 0.0f);
                SoundManager::GetInstance().SetSoundVol("POWER_UP_APPEARS", 0.0f);
                SoundManager::GetInstance().SetSoundVol("ENEMY_DEATH", 0.0f);
                SoundManager::GetInstance().SetSoundVol("MARIO_JUMP", 0.0f);
                SoundManager::GetInstance().SetSoundVol("BUTTON_CLICK", 0.0f);
                std::cout << "[SettingsScreen] Muted: musicVolume and sfxVolume set to 0" << std::endl;
            } else {
                // Restore default volumes when unmuted (optional, adjust as needed)
                musicVolume = 50;
                sfxVolume = 50;
                SoundManager::GetInstance().SetMusicVol("MENU", musicVolume / 100.0f);
                SoundManager::GetInstance().SetMusicVol("GAMEWORLD_1", musicVolume / 100.0f);
                SoundManager::GetInstance().SetSoundVol("COIN_COLLECTION", sfxVolume / 100.0f);
                SoundManager::GetInstance().SetSoundVol("POWER_UP_APPEARS", sfxVolume / 100.0f);
                SoundManager::GetInstance().SetSoundVol("ENEMY_DEATH", sfxVolume / 100.0f);
                SoundManager::GetInstance().SetSoundVol("MARIO_JUMP", sfxVolume / 100.0f);
                SoundManager::GetInstance().SetSoundVol("BUTTON_CLICK", sfxVolume / 100.0f);
                std::cout << "[SettingsScreen] Unmuted: musicVolume and sfxVolume restored to 50" << std::endl;
            }
        }
        
        if (backButton.IsPressed()) {
            if (!SoundManager::GetInstance().IsMuted()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            screenController->ChangeScreen(new MenuScreen(screenController));
        }

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            std::cout << "[Debug] Mouse pos: (" << mousePos.x << ", " << mousePos.y 
                      << "), musicSliderRect: (" << musicSliderRect.x << ", " << musicSliderRect.y 
                      << ", " << musicSliderRect.width << ", " << musicSliderRect.height << ")" 
                      << ", collision: " << CheckCollisionPointRec(GetMousePosition(), musicSliderRect) << std::endl;
        }
    }
}

void SettingsScreen::Draw() {
    DrawTexturePro(*backgroundTexture, 
                   Rectangle{0, 0, (float)backgroundTexture->width, (float)backgroundTexture->height},
                   Rectangle{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                   Vector2{0, 0}, 0.0f, WHITE);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.3f));

    if (showTutorial) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(RAYWHITE, 0.95f));
        float scale = 0.8f;
        int scaledWidth = static_cast<int>(tutorialImage.width * scale);
        int scaledHeight = static_cast<int>(tutorialImage.height * scale);
        int imageX = (GetScreenWidth() - scaledWidth) / 2;
        int imageY = (GetScreenHeight() - scaledHeight) / 2;

        DrawTextureEx(
            tutorialImage,
            Vector2{ static_cast<float>(imageX), static_cast<float>(imageY) },
            0.0f,
            scale,
            WHITE
        );

        Vector2 defaultBtnSize = {40, 40};
        closeTutorialButton.SetSize(defaultBtnSize);
        Vector2 closeBtnSize = closeTutorialButton.GetSize();
        float margin = 12.0f;

        float closeBtnX = imageX + scaledWidth - closeBtnSize.x - margin;
        float closeBtnY = imageY + margin;

        closeTutorialButton.SetPosition({ closeBtnX, closeBtnY });
        closeTutorialButton.Draw();
    } else {
        DrawTexturePro(*settingInterfaceTexture, 
                       Rectangle{0, 0, (float)settingInterfaceTexture->width, (float)settingInterfaceTexture->height},
                       Rectangle{(float)GetScreenWidth()/2 - 450, (float)GetScreenHeight()/2 - 300, 1000, 600},
                       Vector2{0, 0}, 0.0f, WHITE);
        
        tutorialsButton.Draw();
        audioSettingButton.Draw();
        
        int screenW = GetScreenWidth();
        int centerGap = (450 + 600) / 2;
        int sliderStartY = centerGap - 45;
        int sliderW = 250;
        int sliderX = screenW/2 - sliderW/2;
        
        int musicSliderY = sliderStartY;
        DrawRectangle(sliderX - 10, musicSliderY - 15, sliderW + 20, 50, Fade(DARKGRAY, 0.3f));
        DrawText("Music", sliderX, musicSliderY - 10, 18, WHITE);
        DrawRectangle(sliderX, musicSliderY + 15, sliderW, 8, LIGHTGRAY);
        DrawRectangle(sliderX, musicSliderY + 15, (int)(musicVolume * (sliderW/100.0f)), 8, SKYBLUE);
        int musicKnobX = sliderX + (int)(musicVolume * (sliderW/100.0f));
        DrawCircle(musicKnobX, musicSliderY + 19, 12, WHITE);
        DrawCircle(musicKnobX, musicSliderY + 19, 10, BLUE);
        DrawText(TextFormat("%d%%", musicVolume), sliderX + sliderW + 20, musicSliderY + 10, 18, WHITE);

        int sfxSliderY = sliderStartY + 70;
        DrawRectangle(sliderX - 10, sfxSliderY - 15, sliderW + 20, 50, Fade(DARKGRAY, 0.3f));
        DrawText("Sound Effects", sliderX, sfxSliderY - 10, 18, WHITE);
        DrawRectangle(sliderX, sfxSliderY + 15, sliderW, 8, LIGHTGRAY);
        DrawRectangle(sliderX, sfxSliderY + 15, (int)(sfxVolume * (sliderW/100.0f)), 8, ORANGE);
        int sfxKnobX = sliderX + (int)(sfxVolume * (sliderW/100.0f));
        DrawCircle(sfxKnobX, sfxSliderY + 19, 12, WHITE);
        DrawCircle(sfxKnobX, sfxSliderY + 19, 10, ORANGE);
        DrawText(TextFormat("%d%%", sfxVolume), sliderX + sliderW + 20, sfxSliderY + 10, 18, WHITE);

        muteToggleButton.Draw();
        backButton.Draw();
    }
}