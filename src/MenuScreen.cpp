
#include "MenuScreen.h"
#include "ResrcManager.h"
#include "GameScreen.h"
<<<<<<< HEAD
#include "GameModeScreen.h"
=======
#include "MapEditorScreen.h"
>>>>>>> origin/mapedit
MenuScreen::MenuScreen(ScreenController* screenController ) : Screen(screenController), 
startButton(Vector2{1600/2 - 100,900/2 - 50}, Vector2{200, 50}),
MapEditorButton(Vector2{1600/2 - 228/2 ,900/2 + 60}, Vector2{227, 50}) {
    backgroundTexture = &ResrcManager::GetInstance().getTexture("MENU_BACKGROUND");
    startButton.SetTexture(ResrcManager::GetInstance().getTexture("START_BUTTON"));
    MapEditorButton.SetTexture(ResrcManager::GetInstance().getTexture("MAP_EDITOR_BUTTON"));
    // startButton.SetPosition({1600/2 - 50, 900 / 2 - 50});
    // startButton.SetSize({200, 50});
    // exitButton.SetTexture(ResrcManager::GetInstance().getTexture("EXIT_BUTTON"));
    // exitButton.SetPosition({1600 / 2 - 100, 900 / 2 + 10});
    // exitButton.SetSize({200, 50});
    // settingsButton.SetTexture(ResrcManager::GetInstance().getTexture("SETTINGS_BUTTON"));
    // settingsButton.SetPosition({1600 / 2 - 100, 900 / 2 + 70});
    // settingsButton.SetSize({200, 50});
}

void MenuScreen::Update() {
    startButton.Update();
    MapEditorButton.Update();
    // exitButton.Update();
    // settingsButton.Update();

    if (startButton.IsPressed()) {
        std::cout << "Start button pressed!" << std::endl;
<<<<<<< HEAD
        screenController->ChangeScreen(new GameModeScreen(screenController));}
=======
        screenController->ChangeScreen(new GameScreen(screenController));}
    else if (MapEditorButton.IsPressed()) {
        std::cout << "Map Editor button pressed!" << std::endl;
        screenController->ChangeScreen(new MapEditorScreen(screenController));}
>>>>>>> origin/mapedit
         // Debug message
    // } else if (exitButton.IsPressed()) {
    //     CloseWindow(); // Close the window and exit the application
    // } else if (settingsButton.IsPressed()) {
    //     // Handle settings button press
    //     // For now, we can just print a message or open a settings screen
    //     std::cout << "Settings button pressed!" << std::endl;
    // }
}

void MenuScreen::Draw() {

    DrawTexturePro(*backgroundTexture,Rectangle{0, 0, (float)backgroundTexture->width, (float)backgroundTexture->height},Rectangle{0, 0, 1600, 900},Vector2{0, 0}, 0.0f, WHITE);
    startButton.Draw();
    MapEditorButton.Draw();

    // exitButton.Draw();
    // settingsButton.Draw();
}


