#include "SettingsScreen.h"
#include "ResrcManager.h"
#include <raylib.h>
#include <string>
#include "GameScreen.h"
#include "MenuScreen.h"
#include "Screen.h"
SettingsScreen::SettingsScreen(ScreenController *controller)
    : Screen(controller),
      backButton(Vector2{30, 30}, Vector2{122, 50}, "Back"),
      tutorialsButton(Vector2{550, 100}, Vector2{240, 40}, "Tutorials"),
      musicMinusButton(Vector2{600, 300}, Vector2{40, 40}, "-"),
      musicPlusButton(Vector2{800, 300}, Vector2{40, 40}, "+"),
      sfxMinusButton(Vector2{600, 370}, Vector2{40, 40}, "-"),
      sfxPlusButton(Vector2{800, 370}, Vector2{40, 40}, "+"),
      muteToggleButton(Vector2{600, 440}, Vector2{240, 40}, "Mute All: Off"),
    //   pauseButton(Vector2{600, 520}, Vector2{240, 50}, "Pause Game"),
      musicVolume(50),
      sfxVolume(50),
      isMuted(false),
    //   isPaused(false),
      showTutorial(false)
{
    tutorialImage = LoadTexture("../resources/Menu/Tutorials.png"); 
    closeTutorialButton = Button(Vector2{100, 100}, Vector2{40, 40}, "X");
    closeTutorialTexture = LoadTexture("resources/Menu/CloseX.png");
}

SettingsScreen::~SettingsScreen()
{
    UnloadTexture(tutorialImage);
    UnloadTexture(closeTutorialTexture);
}

void SettingsScreen::Update()
{
    if (showTutorial)
    {
        closeTutorialButton.Update();
        tutorialsButton.Update();
        if (closeTutorialButton.IsPressed())
        {
            showTutorial = false;
            tutorialsButton.SetText("Tutorials");
        }
        // return;
    }
    else
    {

        // backButton.Update();
        // tutorialsButton.Update();
        // musicMinusButton.Update();
        // musicPlusButton.Update();
        // sfxMinusButton.Update();
        // sfxPlusButton.Update();
        // muteToggleButton.Update();
        // pauseButton.Update();

        if (backButton.IsPressed())
        {
            screenController->ChangeScreen(new MenuScreen(screenController));
        }
        if (tutorialsButton.IsPressed())
        {
            showTutorial = !showTutorial;
            tutorialsButton.SetText(showTutorial ? "Hide Tutorials" : "Tutorials");
        }
        if (musicMinusButton.IsPressed() && musicVolume > 0)
            musicVolume -= 5;
        if (musicPlusButton.IsPressed() && musicVolume < 100)
            musicVolume += 5;
        if (sfxMinusButton.IsPressed() && sfxVolume > 0)
            sfxVolume -= 5;
        if (sfxPlusButton.IsPressed() && sfxVolume < 100)
            sfxVolume += 5;
        if (muteToggleButton.IsPressed())
        {
            isMuted = !isMuted;
            muteToggleButton.SetText(isMuted ? "Mute All: ON" : "Mute All: OFF");
        }
        // if (pauseButton.IsPressed())
        // {
        //     isPaused = !isPaused;
        //     pauseButton.SetText(isPaused ? "Resume Game" : "Pause Game");
        // }
    }
}

// Draw the settings screen
void SettingsScreen::Draw() {
    ClearBackground(RAYWHITE);

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
        // Centered layout
        int screenW = GetScreenWidth();
        int y = 60;
        DrawText("Settings", screenW/2 - MeasureText("Settings", 40)/2, y, 40, BLACK);
        y += 60;

        // Tutorials button
        Vector2 tutBtnSize = tutorialsButton.GetSize();
        tutorialsButton.SetPosition({ (float)(screenW/2 - tutBtnSize.x/2), (float)y });
        tutorialsButton.Draw();
        y += (int)tutBtnSize.y + 40;

        // Audio settings (center all controls)
        int audioBlockW = 400;
        int audioBlockX = screenW/2 - audioBlockW/2;
        int audioY = y;
        DrawText("Audio Settings:", screenW/2 - MeasureText("Audio Settings:", 32)/2, audioY, 32, BLACK);
        audioY += 40;

        // Music volume controls
        int sliderW = 180;
        int sliderX = screenW/2 - sliderW/2;
        DrawText("Music Volume", sliderX, audioY, 22, DARKGRAY);
        musicMinusButton.SetPosition({ (float)(sliderX - 50), (float)(audioY - 10) });
        musicPlusButton.SetPosition({ (float)(sliderX + sliderW + 10), (float)(audioY - 10) });
        musicMinusButton.Draw();
        musicPlusButton.Draw();
        DrawRectangle(sliderX, audioY + 30, sliderW, 10, LIGHTGRAY);
        DrawRectangle(sliderX, audioY + 30, (int)(musicVolume * (sliderW/100.0f)), 10, BLUE);
        DrawText(TextFormat("%d", musicVolume), sliderX + sliderW + 50, audioY, 22, BLACK);
        audioY += 50;

        // SFX volume controls
        DrawText("Sound Effects Volume", sliderX, audioY, 22, DARKGRAY);
        sfxMinusButton.SetPosition({ (float)(sliderX - 50), (float)(audioY - 10) });
        sfxPlusButton.SetPosition({ (float)(sliderX + sliderW + 10), (float)(audioY - 10) });
        sfxMinusButton.Draw();
        sfxPlusButton.Draw();
        DrawRectangle(sliderX, audioY + 30, sliderW, 10, LIGHTGRAY);
        DrawRectangle(sliderX, audioY + 30, (int)(sfxVolume * (sliderW/100.0f)), 10, ORANGE);
        DrawText(TextFormat("%d", sfxVolume), sliderX + sliderW + 50, audioY, 22, BLACK);
        audioY += 50;

        // Mute button
        Vector2 muteBtnSize = muteToggleButton.GetSize();
        muteToggleButton.SetPosition({ (float)(screenW/2 - muteBtnSize.x/2), (float)audioY });
        muteToggleButton.Draw();
        y = audioY + (int)muteBtnSize.y + 40;

        // Pause button
        // Vector2 pauseBtnSize = pauseButton.GetSize();
        // pauseButton.SetPosition({ (float)(screenW/2 - pauseBtnSize.x/2), (float)y });
        // pauseButton.Draw();
        // y += (int)pauseBtnSize.y + 30;

        backButton.SetPosition({30, 30});
        backButton.Draw();
    }
}


void SettingsScreen::DrawTutorials()
{
    DrawTexture(tutorialImage, 400, 80, WHITE); 
    closeTutorialButton.Draw();
    // Vector2 closeBtnPos = closeTutorialButton.GetPosition({400, 800});
    // DrawTexture(closeTutorialTexture, closeBtnPos.x, closeBtnPos.y, WHITE);
}

void SettingsScreen::DrawAudioSettings()
{
    DrawText("Audio Settings:", 550, 300, 32, BLACK);

    DrawMusicVolumeControls();
    DrawSFXVolumeControls();

    muteToggleButton.Draw();
}

void SettingsScreen::DrawMusicVolumeControls()
{
    DrawText("Music Volume", 650, 340, 22, DARKGRAY);
    musicMinusButton.Draw();
    musicPlusButton.Draw();
    DrawRectangle(650, 370, 150, 10, LIGHTGRAY);
    DrawRectangle(650, 370, musicVolume * 1.5f, 10, BLUE);
    DrawText(TextFormat("%d", musicVolume), 810, 340, 22, BLACK);
}

void SettingsScreen::DrawSFXVolumeControls()
{
    DrawText("Sound Effects Volume", 650, 410, 22, DARKGRAY);
    sfxMinusButton.Draw();
    sfxPlusButton.Draw();
    DrawRectangle(650, 440, 150, 10, LIGHTGRAY);
    DrawRectangle(650, 440, sfxVolume * 1.5f, 10, ORANGE);
    DrawText(TextFormat("%d", sfxVolume), 810, 410, 22, BLACK);
}

// void SettingsScreen::DrawPauseButton()
// {
//     DrawText("Pause Game:", 550, 530, 32, BLACK);
//     pauseButton.Draw();
// }