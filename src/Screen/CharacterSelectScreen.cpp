#include "../inc/Screen/CharacterSelectScreen.h"
#include "../inc/Screen/GameModeScreen.h"
#include "../inc/Screen/MapSelectScreen.h"
#include "ResrcManager.h"
#include "SoundManager.h"

CharacterSelectScreen::CharacterSelectScreen(ScreenController* screenController, bool multiplayer)
    : Screen(screenController),
      isMultiplayer(multiplayer),
      marioButton(Vector2{(float)GetScreenWidth()/2 - 350, (float)GetScreenHeight()/2 - 80}, Vector2{150, 150}),
      luigiButton(Vector2{(float)GetScreenWidth()/2 - 125, (float)GetScreenHeight()/2 - 80}, Vector2{150, 150}),
      toadButton(Vector2{(float)GetScreenWidth()/2 + 100, (float)GetScreenHeight()/2 - 80}, Vector2{150, 150}),
      peachButton(Vector2{(float)GetScreenWidth()/2 + 325, (float)GetScreenHeight()/2 - 80}, Vector2{150, 150}),
      backButton(Vector2{50, 50}, Vector2{80, 80}),
      startButton(Vector2{(float)GetScreenWidth()/2 - 75, (float)GetScreenHeight()/2 + 150}, Vector2{200, 80}),
      player1Character(CharacterType::MARIO),
      player2Character(CharacterType::LUIGI),
      player1Selected(false),
      player2Selected(false)
{
    backgroundTexture = &ResrcManager::GetInstance().getTexture("BACKGROUND_10");
    chooseCharacterTexture = &ResrcManager::GetInstance().getTexture("CHOOSE CHARACTOR");
    marioButton.SetTexture(ResrcManager::GetInstance().getTexture("MARIO_BUTTON"));
    luigiButton.SetTexture(ResrcManager::GetInstance().getTexture("LUIGI_BUTTON"));
    toadButton.SetTexture(ResrcManager::GetInstance().getTexture("TOAD_BUTTON"));
    peachButton.SetTexture(ResrcManager::GetInstance().getTexture("PEACH_BUTTON"));
    backButton.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
    startButton.SetTexture(ResrcManager::GetInstance().getTexture("START_BUTTON"));
}

CharacterSelectScreen::~CharacterSelectScreen() {
}

void CharacterSelectScreen::Update() {
    marioButton.Update();
    luigiButton.Update();
    toadButton.Update();
    peachButton.Update();
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
    
    if (toadButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        SelectCharacter(CharacterType::TOAD);
    }
    
    if (peachButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        SelectCharacter(CharacterType::PEACH);
    }
    
    // Start game if conditions are met
    if (startButton.IsPressed() && CanStartGame()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        screenController->ChangeScreen(new MapSelectScreen(screenController, isMultiplayer, player1Character, player2Character));
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
    // Draw full screen background texture (bottom layer)
    DrawTexturePro(*backgroundTexture, 
                   Rectangle{0, 0, (float)backgroundTexture->width, (float)backgroundTexture->height},
                   Rectangle{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                   Vector2{0, 0}, 0.0f, WHITE);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.3f));
    
    // Draw "CHOOSE CHARACTER" background texture (centered perfectly)
    DrawTexturePro(*chooseCharacterTexture, 
                   Rectangle{0, 0, (float)chooseCharacterTexture->width, (float)chooseCharacterTexture->height},
                   Rectangle{(float)GetScreenWidth()/2 - 600, (float)GetScreenHeight()/2 - 350, 1200, 600},
                   Vector2{0, 0}, 0.0f, WHITE);
    
    // Draw character buttons
    marioButton.Draw();
    luigiButton.Draw();
    toadButton.Draw();
    peachButton.Draw();
    
    // Draw selection indicators
    if (player1Selected) {
        Color p1Color = RED;
        float p1X = 0;
        switch(player1Character) {
            case CharacterType::MARIO: p1X = (float)GetScreenWidth()/2 - 275; break;
            case CharacterType::LUIGI: p1X = (float)GetScreenWidth()/2 - 50; break;
            case CharacterType::TOAD: p1X = (float)GetScreenWidth()/2 + 175; break;
            case CharacterType::PEACH: p1X = (float)GetScreenWidth()/2 + 400; break;
        }
        DrawText("P1", p1X, (float)GetScreenHeight()/2 + 80, 20, p1Color);
    }
    
    if (isMultiplayer && player2Selected) {
        Color p2Color = BLUE;
        float p2X = 0;
        switch(player2Character) {
            case CharacterType::MARIO: p2X = (float)GetScreenWidth()/2 - 275; break;
            case CharacterType::LUIGI: p2X = (float)GetScreenWidth()/2 - 50; break;
            case CharacterType::TOAD: p2X = (float)GetScreenWidth()/2 + 175; break;
            case CharacterType::PEACH: p2X = (float)GetScreenWidth()/2 + 400; break;
        }
        DrawText("P2", p2X, (float)GetScreenHeight()/2 + 105, 20, p2Color);
    }
    
    // Draw buttons
    if (CanStartGame()) {
        startButton.Draw();
    }
    
    backButton.Draw();
}