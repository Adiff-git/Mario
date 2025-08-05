#include "../inc/Screen/GameModeScreen.h"
#include "../inc/Screen/MenuScreen.h"
#include "../inc/Screen/CharacterSelectScreen.h"
#include "ResrcManager.h"
#include "SoundManager.h"

GameModeScreen::GameModeScreen(ScreenController* screenController)
    : Screen(screenController),
      singlePlayerButton(Vector2{(float)GetScreenWidth()/2 - 125, 300}, Vector2{250, 70}),
      multiPlayerButton(Vector2{(float)GetScreenWidth()/2 - 125, 400}, Vector2{250, 70}),
      backButton(Vector2{50, 50}, Vector2{80, 80})
{
    backgroundTexture = &ResrcManager::GetInstance().getTexture("BACKGROUND_10");
    choosePlayerTexture = &ResrcManager::GetInstance().getTexture("CHOOSE PLAYER");
    singlePlayerButton.SetTexture(ResrcManager::GetInstance().getTexture("1_PLAYER"));
    multiPlayerButton.SetTexture(ResrcManager::GetInstance().getTexture("2_PLAYERS"));
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
    // Draw full screen background texture (bottom layer)
    DrawTexturePro(*backgroundTexture, 
                   Rectangle{0, 0, (float)backgroundTexture->width, (float)backgroundTexture->height},
                   Rectangle{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                   Vector2{0, 0}, 0.0f, WHITE);
    
    // Draw "CHOOSE PLAYER" background texture (centered perfectly)
    DrawTexturePro(*choosePlayerTexture, 
                   Rectangle{0, 0, (float)choosePlayerTexture->width, (float)choosePlayerTexture->height},
                   Rectangle{(float)GetScreenWidth()/2 - 500, (float)GetScreenHeight()/2 - 250, 1000, 500},
                   Vector2{0, 0}, 0.0f, WHITE);
    
    // Draw buttons (these will appear on top of the background)
    singlePlayerButton.Draw();
    multiPlayerButton.Draw();
    backButton.Draw();
}