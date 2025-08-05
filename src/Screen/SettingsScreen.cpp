#include "../inc/Screen/SettingsScreen.h"
#include "../inc/Screen/MenuScreen.h"
#include "../inc/ResrcManager.h"
#include "../inc/SoundManager.h"

SettingsScreen::SettingsScreen(ScreenController* screenController)
    : Screen(screenController),
      tutorialsButton(Vector2{(float)GetScreenWidth()/2 - 120, 310}, Vector2{240, 60}),
      audioSettingButton(Vector2{(float)GetScreenWidth()/2 - 120, 390}, Vector2{240, 60}),
      muteToggleButton(Vector2{(float)GetScreenWidth()/2 - 120, 600}, Vector2{240, 60}),
      backButton(Vector2{50, 50}, Vector2{80, 80}),
      musicVolume(50),
      sfxVolume(50),
      isMuted(false),
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
    muteToggleButton.SetTexture(ResrcManager::GetInstance().getTexture("MUTE ALL ON")); // Không tắt âm → hiển thị ON
    backButton.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
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
        tutorialsButton.Update();
        audioSettingButton.Update();
        muteToggleButton.Update();
        backButton.Update();
        
        if (tutorialsButton.IsPressed()) {
            SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            showTutorial = !showTutorial;
        }
        
        if (audioSettingButton.IsPressed()) {
            SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            showAudioSettings = !showAudioSettings;
        }
        
        if (muteToggleButton.IsPressed()) {
            SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            isMuted = !isMuted;
            muteToggleButton.SetTexture(isMuted ? 
                ResrcManager::GetInstance().getTexture("MUTE ALL OFF") :  // Tắt âm → hiển thị OFF
                ResrcManager::GetInstance().getTexture("MUTE ALL ON"));  // Không tắt âm → hiển thị ON
            
            // muteToggleButton.SetSize(Vector2{240, 80});
        }
        
        if (backButton.IsPressed()) {
            SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            screenController->ChangeScreen(new MenuScreen(screenController));
        }
    }
}

void SettingsScreen::Draw() {
    // Draw full screen background texture (bottom layer)
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
        // Draw larger "SETTING INTERFACE" background texture (centered)
        DrawTexturePro(*settingInterfaceTexture, 
                       Rectangle{0, 0, (float)settingInterfaceTexture->width, (float)settingInterfaceTexture->height},
                       Rectangle{(float)GetScreenWidth()/2 - 450, (float)GetScreenHeight()/2 - 300, 1000, 600},
                       Vector2{0, 0}, 0.0f, WHITE);
        
        // Draw buttons
        tutorialsButton.Draw();
        audioSettingButton.Draw();
        
        // Audio settings with sliders (always visible now, perfectly centered between buttons)
        int screenW = GetScreenWidth();
        // Calculate center position between audioSettingButton (ends at 450) and muteToggleButton (starts at 600)
        int centerGap = (450 + 600) / 2; // Center point = 525
        int sliderStartY = centerGap - 45; // 490 - start first slider 35px above center
        
        // --- MUSIC SLIDER ---
        int sliderW = 250;
        int sliderX = screenW/2 - sliderW/2;
        int musicSliderY = sliderStartY;
        
        // Music slider background with rounded corners effect
        DrawRectangle(sliderX - 10, musicSliderY - 15, sliderW + 20, 50, Fade(DARKGRAY, 0.3f));
        DrawText("Music", sliderX, musicSliderY - 10, 18, WHITE);
        
        // Main slider track
        DrawRectangle(sliderX, musicSliderY + 15, sliderW, 8, LIGHTGRAY);
        // Active portion
        DrawRectangle(sliderX, musicSliderY + 15, (int)(musicVolume * (sliderW/100.0f)), 8, SKYBLUE);
        // Slider knob
        int musicKnobX = sliderX + (int)(musicVolume * (sliderW/100.0f));
        DrawCircle(musicKnobX, musicSliderY + 19, 12, WHITE);
        DrawCircle(musicKnobX, musicSliderY + 19, 10, BLUE);
        
        // Volume text
        DrawText(TextFormat("%d%%", musicVolume), sliderX + sliderW + 20, musicSliderY + 10, 18, WHITE);

        // Mouse interaction for music slider
        Rectangle musicSliderRect = { (float)sliderX, (float)musicSliderY + 5, (float)sliderW, 25 };
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), musicSliderRect)) {
            float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
            percent = std::max(0.0f, std::min(1.0f, percent));
            musicVolume = (int)(percent * 100);
            SoundManager::GetInstance().SetMusicVol("MENU", musicVolume / 100.0f);
        }

        // --- SFX SLIDER ---
        int sfxSliderY = sliderStartY + 70;
        
        // SFX slider background with rounded corners effect
        DrawRectangle(sliderX - 10, sfxSliderY - 15, sliderW + 20, 50, Fade(DARKGRAY, 0.3f));
        DrawText("Sound Effects", sliderX, sfxSliderY - 10, 18, WHITE);
        
        // Main slider track
        DrawRectangle(sliderX, sfxSliderY + 15, sliderW, 8, LIGHTGRAY);
        // Active portion
        DrawRectangle(sliderX, sfxSliderY + 15, (int)(sfxVolume * (sliderW/100.0f)), 8, ORANGE);
        // Slider knob
        int sfxKnobX = sliderX + (int)(sfxVolume * (sliderW/100.0f));
        DrawCircle(sfxKnobX, sfxSliderY + 19, 12, WHITE);
        // DrawCircle(sfxKnobX, sfxSliderY + 19, 10, DARKORANGE);
        
        // Volume text
        DrawText(TextFormat("%d%%", sfxVolume), sliderX + sliderW + 20, sfxSliderY + 10, 18, WHITE);

        // Mouse interaction for SFX slider
        Rectangle sfxSliderRect = { (float)sliderX, (float)sfxSliderY + 5, (float)sliderW, 25 };
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), sfxSliderRect)) {
            float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
            percent = std::max(0.0f, std::min(1.0f, percent));
            sfxVolume = (int)(percent * 100);
            SoundManager::GetInstance().SetSoundVol("COIN", sfxVolume / 100.0f);
        }
        
        // Draw mute button (always positioned below the sliders)
        muteToggleButton.Draw();
        backButton.Draw();
    }
}