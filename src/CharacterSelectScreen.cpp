#include "CharacterSelectScreen.h"
#include "GameModeScreen.h"
#include "GameScreen.h"
#include "ResrcManager.h"
#include "SoundManager.h"

CharacterSelectScreen::CharacterSelectScreen(ScreenController* screenController, bool multiplayer)
    : Screen(screenController),
      isMultiplayer(multiplayer),
      marioButton(Vector2{200, 300}, Vector2{100, 100}),
      luigiButton(Vector2{400, 300}, Vector2{100, 100}),
      backButton(Vector2{50, 50}, Vector2{100, 50}),
      startButton(Vector2{(float)GetScreenWidth()/2 - 75, 500}, Vector2{150, 60}),
      player1Character(CharacterType::MARIO),
      player2Character(CharacterType::LUIGI),
      player1Selected(false),
      player2Selected(false)
{
    marioButton.SetTexture(ResrcManager::GetInstance().getTexture("MARIO_SELECT"));
    luigiButton.SetTexture(ResrcManager::GetInstance().getTexture("LUIGI_SELECT"));
    backButton.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
    startButton.SetTexture(ResrcManager::GetInstance().getTexture("START_BUTTON"));
}

CharacterSelectScreen::~CharacterSelectScreen() {
}

void CharacterSelectScreen::Update() {
    marioButton.Update();
    luigiButton.Update();
    backButton.Update();
    startButton.Update();
    
    // Character selection logic
    if (marioButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        SelectCharacter(CharacterType::MARIO);
    }
    
    if (luigiButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        SelectCharacter(CharacterType::LUIGI);
    }
    
    // Start game if conditions are met
    if (startButton.IsPressed() && CanStartGame()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        screenController->ChangeScreen(new GameScreen(screenController));
    }
    
    if (backButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        screenController->ChangeScreen(new GameModeScreen(screenController));
    }
}

void CharacterSelectScreen::SelectCharacter(CharacterType character) {
    if (!isMultiplayer) {
        // Single player mode
        player1Character = character;
        player1Selected = true;
    } else {
        // Multiplayer mode
        if (!player1Selected) {
            player1Character = character;
            player1Selected = true;
        } else if (!player2Selected) {
            player2Character = character;
            player2Selected = true;
        }
    }
}

bool CharacterSelectScreen::CanStartGame() {
    if (!isMultiplayer) {
        return player1Selected;
    } else {
        return player1Selected && player2Selected;
    }
}

void CharacterSelectScreen::Draw() {
    ClearBackground(DARKBLUE);
    
    // Draw title
    const char* title = isMultiplayer ? "SELECT CHARACTERS" : "SELECT CHARACTER";
    DrawText(title, GetScreenWidth()/2 - 140, 150, 30, WHITE);
    
    // Draw character buttons
    marioButton.Draw();
    luigiButton.Draw();
    
    // Draw character labels
    DrawText("MARIO", 220, 420, 20, WHITE);
    DrawText("LUIGI", 420, 420, 20, WHITE);
    
    // Draw selection indicators
    if (player1Selected) {
        Color p1Color = (player1Character == CharacterType::MARIO) ? RED : GREEN;
        DrawText("P1", player1Character == CharacterType::MARIO ? 235 : 435, 450, 16, p1Color);
    }
    
    if (isMultiplayer && player2Selected) {
        Color p2Color = (player2Character == CharacterType::MARIO) ? RED : GREEN;
        DrawText("P2", player2Character == CharacterType::MARIO ? 235 : 435, 470, 16, p2Color);
    }
    
    // Draw instructions
    if (!isMultiplayer) {
        DrawText("Click on a character to select", GetScreenWidth()/2 - 120, 200, 16, YELLOW);
    } else {
        if (!player1Selected) {
            DrawText("Player 1: Select your character", GetScreenWidth()/2 - 120, 200, 16, YELLOW);
        } else if (!player2Selected) {
            DrawText("Player 2: Select your character", GetScreenWidth()/2 - 120, 200, 16, YELLOW);
        }
    }
    
    // Draw buttons
    if (CanStartGame()) {
        startButton.Draw();
        DrawText("START", GetScreenWidth()/2 - 25, 520, 20, BLACK);
    }
    
    backButton.Draw();
    DrawText("BACK", 75, 65, 16, BLACK);
}