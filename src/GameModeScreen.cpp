#include "GameModeScreen.h"
#include "MenuScreen.h"
#include "CharacterSelectScreen.h"
#include "ResrcManager.h"
#include "SoundManager.h"

GameModeScreen::GameModeScreen(ScreenController* screenController)
    : Screen(screenController),
      singlePlayerButton(Vector2{(float)GetScreenWidth()/2 - 100, 250}, Vector2{200, 60}),
      multiPlayerButton(Vector2{(float)GetScreenWidth()/2 - 100, 350}, Vector2{200, 60}),
      backButton(Vector2{50, 50}, Vector2{100, 50})
{
    singlePlayerButton.SetTexture(ResrcManager::GetInstance().getTexture("SINGLE_PLAYER_BUTTON"));
    multiPlayerButton.SetTexture(ResrcManager::GetInstance().getTexture("MULTI_PLAYER_BUTTON"));
    backButton.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
}

GameModeScreen::~GameModeScreen() {
}

void GameModeScreen::Update() {
    singlePlayerButton.Update();
    multiPlayerButton.Update();
    backButton.Update();
    
    if (singlePlayerButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        screenController->ChangeScreen(new CharacterSelectScreen(screenController, false)); // false = single player
    }
    
    if (multiPlayerButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        screenController->ChangeScreen(new CharacterSelectScreen(screenController, true)); // true = multiplayer
    }
    
    if (backButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        screenController->ChangeScreen(new MenuScreen(screenController));
    }
}

void GameModeScreen::Draw() {
    ClearBackground(SKYBLUE);
    
    // Draw title
    DrawText("SELECT GAME MODE", GetScreenWidth()/2 - 150, 150, 30, WHITE);
    
    // Draw buttons
    singlePlayerButton.Draw();
    multiPlayerButton.Draw();
    backButton.Draw();
    
    // Draw button labels
    DrawText("1 PLAYER", GetScreenWidth()/2 - 50, 270, 20, BLACK);
    DrawText("2 PLAYERS", GetScreenWidth()/2 - 55, 370, 20, BLACK);
    DrawText("BACK", 75, 65, 16, BLACK);
}