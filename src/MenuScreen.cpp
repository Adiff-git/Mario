
#include "MenuScreen.h"
#include "ResrcManager.h"
// #include "Map1Screen.h"
MenuScreen::MenuScreen(ScreenManager* screenManager ) : Screen(screenManager) {
    backgroundTexture = &ResrcManager::GetInstance().getTexture("MENU_BACKGROUND");
    startButton.SetTexture(ResrcManager::GetInstance().getTexture("START_BUTTON"));
    startButton.SetPosition({1600 / 2 - 100, 900 / 2 - 50});
    startButton.SetSize({200, 50});
    exitButton.SetTexture(ResrcManager::GetInstance().getTexture("EXIT_BUTTON"));
    exitButton.SetPosition({1600 / 2 - 100, 900 / 2 + 10});
    exitButton.SetSize({200, 50});
    settingsButton.SetTexture(ResrcManager::GetInstance().getTexture("SETTINGS_BUTTON"));
    settingsButton.SetPosition({1600 / 2 - 100, 900 / 2 + 70});
    settingsButton.SetSize({200, 50});
}

void MenuScreen::Update() {
    startButton.Update();
    exitButton.Update();
    settingsButton.Update();

    if (startButton.IsPressed()) {
        // screenController->ChangeScreen(new Map1Screen());
    } else if (exitButton.IsPressed()) {
        CloseWindow(); // Close the window and exit the application
    } else if (settingsButton.IsPressed()) {
        // Handle settings button press
        // For now, we can just print a message or open a settings screen
        std::cout << "Settings button pressed!" << std::endl;
    }
}

void MenuScreen::Draw() {
    DrawTextureNPatch(*backgroundTexture, NPatchInfo{0, 0, 0, 0, 0}, Rectangle{0, 0, 1600, 900}, Vector2{0, 0}, 0.0f, WHITE);
    startButton.Draw();
    // exitButton.Draw();
    // settingsButton.Draw();
}


