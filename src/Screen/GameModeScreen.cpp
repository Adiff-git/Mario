#include "../inc/Screen/GameModeScreen.h"
#include "../inc/Screen/MenuScreen.h"
#include "../inc/Screen/CharacterSelectScreen.h"
#include "ResrcManager.h"
#include "SoundManager.h"

GameModeScreen::GameModeScreen(ScreenController* screenController)
    : Screen(screenController),
      singlePlayerButton(Vector2{(float)GetScreenWidth()/2 - 225, 290}, Vector2{200, 267}),
      multiPlayerButton(Vector2{(float)GetScreenWidth()/2 + 25, 290}, Vector2{200, 267}),
      backButton(Vector2{50, 50}, Vector2{80, 80}),
      singlePlayerHovered(false),
      multiPlayerHovered(false)
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
    
    // Update hover states
    singlePlayerHovered = singlePlayerButton.IsHovered();
    multiPlayerHovered = multiPlayerButton.IsHovered();
    
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
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.3f));
    
    // Draw "CHOOSE PLAYER" background texture (centered perfectly)
    DrawTexturePro(*choosePlayerTexture, 
                   Rectangle{0, 0, (float)choosePlayerTexture->width, (float)choosePlayerTexture->height},
                   Rectangle{(float)GetScreenWidth()/2 - 450, (float)GetScreenHeight()/2 - 300, 1000, 500},
                   Vector2{0, 0}, 0.0f, WHITE);
    
    // Draw buttons (these will appear on top of the background)
    singlePlayerButton.Draw();
    multiPlayerButton.Draw();
    backButton.Draw();
    
    // Draw hover text effects
    if (singlePlayerHovered) {
        DrawText("1 PLAYER", (float)GetScreenWidth()/2 - 225 + 50, 290 + 280, 24, YELLOW);
        // Draw background rectangle for better readability
        DrawRectangle((float)GetScreenWidth()/2 - 225 + 45, 290 + 275, 120, 35, Fade(BLACK, 0.7f));
        DrawText("1 PLAYER", (float)GetScreenWidth()/2 - 225 + 50, 290 + 280, 24, YELLOW);
    }
    
    if (multiPlayerHovered) {
        DrawText("2 PLAYERS", (float)GetScreenWidth()/2 + 25 + 40, 290 + 280, 24, YELLOW);
        // Draw background rectangle for better readability
        DrawRectangle((float)GetScreenWidth()/2 + 25 + 35, 290 + 275, 145, 35, Fade(BLACK, 0.7f));
        DrawText("2 PLAYERS", (float)GetScreenWidth()/2 + 25 + 40, 290 + 280, 24, YELLOW);
    }
}