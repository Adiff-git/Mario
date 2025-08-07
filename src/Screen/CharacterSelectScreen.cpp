#include "../inc/Screen/CharacterSelectScreen.h"
#include "../inc/Screen/GameModeScreen.h"
#include "../inc/Screen/MapSelectScreen.h"
#include "ResrcManager.h"
#include "SoundManager.h"

CharacterSelectScreen::CharacterSelectScreen(ScreenController* screenController, bool multiplayer)
    : Screen(screenController),
      isMultiplayer(multiplayer),
      marioButton(Vector2{(float)GetScreenWidth()/2 - 417.5f, (float)GetScreenHeight()/2 - 100}, Vector2{150, 200}),
      luigiButton(Vector2{(float)GetScreenWidth()/2 - 192.5f, (float)GetScreenHeight()/2 - 100}, Vector2{150, 200}),
      toadButton(Vector2{(float)GetScreenWidth()/2 + 32.5f, (float)GetScreenHeight()/2 - 100}, Vector2{150, 200}),
      peachButton(Vector2{(float)GetScreenWidth()/2 + 257.5f, (float)GetScreenHeight()/2 - 100}, Vector2{150, 200}),
      backButton(Vector2{50, 50}, Vector2{80, 80}),
      startButton(Vector2{(float)GetScreenWidth()/2 - 75, (float)GetScreenHeight()/2 + 150}, Vector2{200, 80}),
      player1Character(CharacterType::MARIO),
      player2Character(CharacterType::LUIGI),
      player1Selected(false),
      player2Selected(false),
      marioHovered(false),
      luigiHovered(false),
      toadHovered(false),
      peachHovered(false),
      marioScale(1.0f),
      luigiScale(1.0f),
      toadScale(1.0f),
      peachScale(1.0f),
      nextSelectionForPlayer1(true)
{
    backgroundTexture = &ResrcManager::GetInstance().getTexture("BACKGROUND_10");
    chooseCharacterTexture = &ResrcManager::GetInstance().getTexture("CHOOSE CHARACTOR");
    pixelFont = &ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT");
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
    
    // Update hover states
    marioHovered = marioButton.IsHovered();
    luigiHovered = luigiButton.IsHovered();
    toadHovered = toadButton.IsHovered();
    peachHovered = peachButton.IsHovered();
    
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
        // Reset all scales first
        marioScale = luigiScale = toadScale = peachScale = 1.0f;
        
        player1Character = character;
        player1Selected = true;
        
        // Scale up selected character
        switch(character) {
            case CharacterType::MARIO: marioScale = 1.2f; break;
            case CharacterType::LUIGI: luigiScale = 1.2f; break;
            case CharacterType::TOAD: toadScale = 1.2f; break;
            case CharacterType::PEACH: peachScale = 1.2f; break;
        }
    } else {
        // Multiplayer mode
        if (!player1Selected) {
            // First player selecting
            marioScale = luigiScale = toadScale = peachScale = 1.0f;
            
            player1Character = character;
            player1Selected = true;
            nextSelectionForPlayer1 = false; // Next selection will be for Player 2
            
            // Scale up selected character
            switch(character) {
                case CharacterType::MARIO: marioScale = 1.2f; break;
                case CharacterType::LUIGI: luigiScale = 1.2f; break;
                case CharacterType::TOAD: toadScale = 1.2f; break;
                case CharacterType::PEACH: peachScale = 1.2f; break;
            }
        } else if (!player2Selected) {
            // Second player selecting
            player2Character = character;
            player2Selected = true;
            nextSelectionForPlayer1 = true; // Next selection will be for Player 1
            
            // Scale up second selected character (but don't reset first)
            switch(character) {
                case CharacterType::MARIO: marioScale = 1.2f; break;
                case CharacterType::LUIGI: luigiScale = 1.2f; break;
                case CharacterType::TOAD: toadScale = 1.2f; break;
                case CharacterType::PEACH: peachScale = 1.2f; break;
            }
        } else {
            // Both players already selected, someone is changing character
            // Check if this character is already selected by player 1 or 2
            if (character == player1Character && character == player2Character) {
                // Both players have the same character, toggle who gets the new selection
                if (nextSelectionForPlayer1) {
                    // Player 1 is changing, but character is same as current P1
                    nextSelectionForPlayer1 = false; // Switch to P2 next time
                } else {
                    // Player 2 is changing, but character is same as current P2
                    nextSelectionForPlayer1 = true; // Switch to P1 next time
                }
                return;
            } else if (character == player1Character) {
                // Clicking on Player 1's current character
                if (nextSelectionForPlayer1) {
                    // Player 1 clicking their own character - no change
                    nextSelectionForPlayer1 = false; // Switch to P2 next time
                    return;
                } else {
                    // Player 2 wants Player 1's character - swap them
                    CharacterType temp = player1Character;
                    player1Character = player2Character;
                    player2Character = temp;
                    nextSelectionForPlayer1 = true; // Switch to P1 next time
                }
            } else if (character == player2Character) {
                // Clicking on Player 2's current character
                if (!nextSelectionForPlayer1) {
                    // Player 2 clicking their own character - no change
                    nextSelectionForPlayer1 = true; // Switch to P1 next time
                    return;
                } else {
                    // Player 1 wants Player 2's character - swap them
                    CharacterType temp = player1Character;
                    player1Character = player2Character;
                    player2Character = temp;
                    nextSelectionForPlayer1 = false; // Switch to P2 next time
                }
            } else {
                // Selecting a completely new character
                if (nextSelectionForPlayer1) {
                    // Player 1 is changing their character
                    player1Character = character;
                    nextSelectionForPlayer1 = false; // Switch to P2 next time
                } else {
                    // Player 2 is changing their character
                    player2Character = character;
                    nextSelectionForPlayer1 = true; // Switch to P1 next time
                }
            }
            
            // Reset all scales and re-scale selected characters
            marioScale = luigiScale = toadScale = peachScale = 1.0f;
            
            // Scale Player 1's character
            switch(player1Character) {
                case CharacterType::MARIO: marioScale = 1.2f; break;
                case CharacterType::LUIGI: luigiScale = 1.2f; break;
                case CharacterType::TOAD: toadScale = 1.2f; break;
                case CharacterType::PEACH: peachScale = 1.2f; break;
            }
            
            // Scale Player 2's character (may be the same as Player 1)
            switch(player2Character) {
                case CharacterType::MARIO: marioScale = 1.2f; break;
                case CharacterType::LUIGI: luigiScale = 1.2f; break;
                case CharacterType::TOAD: toadScale = 1.2f; break;
                case CharacterType::PEACH: peachScale = 1.2f; break;
            }
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
                   Rectangle{(float)GetScreenWidth()/2 - 530, (float)GetScreenHeight()/2 - 350, 1200, 600},
                   Vector2{0, 0}, 0.0f, WHITE);
    
    // Draw thick black frames for character buttons (scaled)
    // Mario button frame
    float marioFrameWidth = 160 * marioScale;
    float marioFrameHeight = 210 * marioScale;
    float marioFrameX = (float)GetScreenWidth()/2 - 417.5f - (marioFrameWidth - 150)/2;
    float marioFrameY = (float)GetScreenHeight()/2 - 100 - (marioFrameHeight - 200)/2;
    DrawRectangleLines(marioFrameX - 5, marioFrameY - 5, marioFrameWidth + 10, marioFrameHeight + 10, BLACK);
    DrawRectangleLines(marioFrameX - 3, marioFrameY - 3, marioFrameWidth + 6, marioFrameHeight + 6, BLACK);
    DrawRectangleLines(marioFrameX - 1, marioFrameY - 1, marioFrameWidth + 2, marioFrameHeight + 2, BLACK);
    
    // Luigi button frame
    float luigiFrameWidth = 160 * luigiScale;
    float luigiFrameHeight = 210 * luigiScale;
    float luigiFrameX = (float)GetScreenWidth()/2 - 192.5f - (luigiFrameWidth - 150)/2;
    float luigiFrameY = (float)GetScreenHeight()/2 - 100 - (luigiFrameHeight - 200)/2;
    DrawRectangleLines(luigiFrameX - 5, luigiFrameY - 5, luigiFrameWidth + 10, luigiFrameHeight + 10, BLACK);
    DrawRectangleLines(luigiFrameX - 3, luigiFrameY - 3, luigiFrameWidth + 6, luigiFrameHeight + 6, BLACK);
    DrawRectangleLines(luigiFrameX - 1, luigiFrameY - 1, luigiFrameWidth + 2, luigiFrameHeight + 2, BLACK);
    
    // Toad button frame
    float toadFrameWidth = 160 * toadScale;
    float toadFrameHeight = 210 * toadScale;
    float toadFrameX = (float)GetScreenWidth()/2 + 32.5f - (toadFrameWidth - 150)/2;
    float toadFrameY = (float)GetScreenHeight()/2 - 100 - (toadFrameHeight - 200)/2;
    DrawRectangleLines(toadFrameX - 5, toadFrameY - 5, toadFrameWidth + 10, toadFrameHeight + 10, BLACK);
    DrawRectangleLines(toadFrameX - 3, toadFrameY - 3, toadFrameWidth + 6, toadFrameHeight + 6, BLACK);
    DrawRectangleLines(toadFrameX - 1, toadFrameY - 1, toadFrameWidth + 2, toadFrameHeight + 2, BLACK);
    
    // Peach button frame
    float peachFrameWidth = 160 * peachScale;
    float peachFrameHeight = 210 * peachScale;
    float peachFrameX = (float)GetScreenWidth()/2 + 257.5f - (peachFrameWidth - 150)/2;
    float peachFrameY = (float)GetScreenHeight()/2 - 100 - (peachFrameHeight - 200)/2;
    DrawRectangleLines(peachFrameX - 5, peachFrameY - 5, peachFrameWidth + 10, peachFrameHeight + 10, BLACK);
    DrawRectangleLines(peachFrameX - 3, peachFrameY - 3, peachFrameWidth + 6, peachFrameHeight + 6, BLACK);
    DrawRectangleLines(peachFrameX - 1, peachFrameY - 1, peachFrameWidth + 2, peachFrameHeight + 2, BLACK);
    
    // Draw character buttons with scaling
    // Mario button
    Vector2 marioPos = {(float)GetScreenWidth()/2 - 417.5f, (float)GetScreenHeight()/2 - 100};
    Vector2 marioSize = {150 * marioScale, 200 * marioScale};
    Vector2 marioOffset = {marioSize.x/2 - 75, marioSize.y/2 - 100}; // Center the scaled button
    DrawTexturePro(ResrcManager::GetInstance().getTexture("MARIO_BUTTON"),
                   Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("MARIO_BUTTON").width, 
                            (float)ResrcManager::GetInstance().getTexture("MARIO_BUTTON").height},
                   Rectangle{marioPos.x - marioOffset.x, marioPos.y - marioOffset.y, marioSize.x, marioSize.y},
                   Vector2{0, 0}, 0.0f, WHITE);
    
    // Luigi button
    Vector2 luigiPos = {(float)GetScreenWidth()/2 - 192.5f, (float)GetScreenHeight()/2 - 100};
    Vector2 luigiSize = {150 * luigiScale, 200 * luigiScale};
    Vector2 luigiOffset = {luigiSize.x/2 - 75, luigiSize.y/2 - 100};
    DrawTexturePro(ResrcManager::GetInstance().getTexture("LUIGI_BUTTON"),
                   Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("LUIGI_BUTTON").width, 
                            (float)ResrcManager::GetInstance().getTexture("LUIGI_BUTTON").height},
                   Rectangle{luigiPos.x - luigiOffset.x, luigiPos.y - luigiOffset.y, luigiSize.x, luigiSize.y},
                   Vector2{0, 0}, 0.0f, WHITE);
    
    // Toad button
    Vector2 toadPos = {(float)GetScreenWidth()/2 + 32.5f, (float)GetScreenHeight()/2 - 100};
    Vector2 toadSize = {150 * toadScale, 200 * toadScale};
    Vector2 toadOffset = {toadSize.x/2 - 75, toadSize.y/2 - 100};
    DrawTexturePro(ResrcManager::GetInstance().getTexture("TOAD_BUTTON"),
                   Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("TOAD_BUTTON").width, 
                            (float)ResrcManager::GetInstance().getTexture("TOAD_BUTTON").height},
                   Rectangle{toadPos.x - toadOffset.x, toadPos.y - toadOffset.y, toadSize.x, toadSize.y},
                   Vector2{0, 0}, 0.0f, WHITE);
    
    // Peach button
    Vector2 peachPos = {(float)GetScreenWidth()/2 + 257.5f, (float)GetScreenHeight()/2 - 100};
    Vector2 peachSize = {150 * peachScale, 200 * peachScale};
    Vector2 peachOffset = {peachSize.x/2 - 75, peachSize.y/2 - 100};
    DrawTexturePro(ResrcManager::GetInstance().getTexture("PEACH_BUTTON"),
                   Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("PEACH_BUTTON").width, 
                            (float)ResrcManager::GetInstance().getTexture("PEACH_BUTTON").height},
                   Rectangle{peachPos.x - peachOffset.x, peachPos.y - peachOffset.y, peachSize.x, peachSize.y},
                   Vector2{0, 0}, 0.0f, WHITE);
    
    // Draw invisible buttons for interaction (set alpha to 0)
    Color transparent = {255, 255, 255, 0};
    Texture2D originalMarioTexture = ResrcManager::GetInstance().getTexture("MARIO_BUTTON");
    Texture2D originalLuigiTexture = ResrcManager::GetInstance().getTexture("LUIGI_BUTTON");
    Texture2D originalToadTexture = ResrcManager::GetInstance().getTexture("TOAD_BUTTON");
    Texture2D originalPeachTexture = ResrcManager::GetInstance().getTexture("PEACH_BUTTON");
    
    marioButton.SetTexture(originalMarioTexture);
    luigiButton.SetTexture(originalLuigiTexture);
    toadButton.SetTexture(originalToadTexture);
    peachButton.SetTexture(originalPeachTexture);
    
    // Draw buttons with transparency for interaction only
    DrawTexturePro(originalMarioTexture,
                   Rectangle{0, 0, (float)originalMarioTexture.width, (float)originalMarioTexture.height},
                   Rectangle{(float)GetScreenWidth()/2 - 417.5f, (float)GetScreenHeight()/2 - 100, 150, 200},
                   Vector2{0, 0}, 0.0f, transparent);
    DrawTexturePro(originalLuigiTexture,
                   Rectangle{0, 0, (float)originalLuigiTexture.width, (float)originalLuigiTexture.height},
                   Rectangle{(float)GetScreenWidth()/2 - 192.5f, (float)GetScreenHeight()/2 - 100, 150, 200},
                   Vector2{0, 0}, 0.0f, transparent);
    DrawTexturePro(originalToadTexture,
                   Rectangle{0, 0, (float)originalToadTexture.width, (float)originalToadTexture.height},
                   Rectangle{(float)GetScreenWidth()/2 + 32.5f, (float)GetScreenHeight()/2 - 100, 150, 200},
                   Vector2{0, 0}, 0.0f, transparent);
    DrawTexturePro(originalPeachTexture,
                   Rectangle{0, 0, (float)originalPeachTexture.width, (float)originalPeachTexture.height},
                   Rectangle{(float)GetScreenWidth()/2 + 257.5f, (float)GetScreenHeight()/2 - 100, 150, 200},
                   Vector2{0, 0}, 0.0f, transparent);
    
    // Draw hover effects with character names
    if (marioHovered) {
        const char* marioText = "MARIO";
        Vector2 marioSize = MeasureTextEx(*pixelFont, marioText, 20, 1);
        DrawRectangle((float)GetScreenWidth()/2 - 417.5f + 75 - marioSize.x/2 - 10, (float)GetScreenHeight()/2 + 120, marioSize.x + 20, 30, Fade(BLACK, 0.8f));
        DrawTextEx(*pixelFont, marioText, 
                   Vector2{(float)GetScreenWidth()/2 - 417.5f + 75 - marioSize.x/2, (float)GetScreenHeight()/2 + 125}, 
                   20, 1, YELLOW);
    }
    
    if (luigiHovered) {
        const char* luigiText = "LUIGI";
        Vector2 luigiSize = MeasureTextEx(*pixelFont, luigiText, 20, 1);
        DrawRectangle((float)GetScreenWidth()/2 - 192.5f + 75 - luigiSize.x/2 - 10, (float)GetScreenHeight()/2 + 120, luigiSize.x + 20, 30, Fade(BLACK, 0.8f));
        DrawTextEx(*pixelFont, luigiText, 
                   Vector2{(float)GetScreenWidth()/2 - 192.5f + 75 - luigiSize.x/2, (float)GetScreenHeight()/2 + 125}, 
                   20, 1, YELLOW);
    }
    
    if (toadHovered) {
        const char* toadText = "TOAD";
        Vector2 toadSize = MeasureTextEx(*pixelFont, toadText, 20, 1);
        DrawRectangle((float)GetScreenWidth()/2 + 32.5f + 75 - toadSize.x/2 - 10, (float)GetScreenHeight()/2 + 120, toadSize.x + 20, 30, Fade(BLACK, 0.8f));
        DrawTextEx(*pixelFont, toadText, 
                   Vector2{(float)GetScreenWidth()/2 + 32.5f + 75 - toadSize.x/2, (float)GetScreenHeight()/2 + 125}, 
                   20, 1, YELLOW);
    }
    
    if (peachHovered) {
        const char* peachText = "PEACH";
        Vector2 peachSize = MeasureTextEx(*pixelFont, peachText, 20, 1);
        DrawRectangle((float)GetScreenWidth()/2 + 257.5f + 75 - peachSize.x/2 - 10, (float)GetScreenHeight()/2 + 120, peachSize.x + 20, 30, Fade(BLACK, 0.8f));
        DrawTextEx(*pixelFont, peachText, 
                   Vector2{(float)GetScreenWidth()/2 + 257.5f + 75 - peachSize.x/2, (float)GetScreenHeight()/2 + 125}, 
                   20, 1, YELLOW);
    }
    
    // Draw selection indicators
    if (player1Selected) {
        Color p1Color = RED;
        float p1X = 0;
        switch(player1Character) {
            case CharacterType::MARIO: p1X = (float)GetScreenWidth()/2 - 417.5f + 75; break;
            case CharacterType::LUIGI: p1X = (float)GetScreenWidth()/2 - 192.5f + 75; break;
            case CharacterType::TOAD: p1X = (float)GetScreenWidth()/2 + 32.5f + 75; break;
            case CharacterType::PEACH: p1X = (float)GetScreenWidth()/2 + 257.5f + 75; break;
        }
        DrawTextEx(*pixelFont, "P1", 
                   Vector2{p1X - 10, (float)GetScreenHeight()/2 + 170}, 
                   18, 1, p1Color);
    }
    
    if (isMultiplayer && player2Selected) {
        Color p2Color = BLUE;
        float p2X = 0;
        switch(player2Character) {
            case CharacterType::MARIO: p2X = (float)GetScreenWidth()/2 - 417.5f + 75; break;
            case CharacterType::LUIGI: p2X = (float)GetScreenWidth()/2 - 192.5f + 75; break;
            case CharacterType::TOAD: p2X = (float)GetScreenWidth()/2 + 32.5f + 75; break;
            case CharacterType::PEACH: p2X = (float)GetScreenWidth()/2 + 257.5f + 75; break;
        }
        DrawTextEx(*pixelFont, "P2", 
                   Vector2{p2X - 10, (float)GetScreenHeight()/2 + 195}, 
                   18, 1, p2Color);
    }
    
    // Show whose turn it is to select (when both players have selected)
    if (isMultiplayer && player1Selected && player2Selected) {
        const char* turnText = nextSelectionForPlayer1 ? "Player 1's Turn" : "Player 2's Turn";
        Vector2 turnTextSize = MeasureTextEx(*pixelFont, turnText, 20, 1);
        DrawRectangle((float)GetScreenWidth()/2 - turnTextSize.x/2 - 10, 50, turnTextSize.x + 20, 35, Fade(BLACK, 0.8f));
        DrawTextEx(*pixelFont, turnText, 
                   Vector2{(float)GetScreenWidth()/2 - turnTextSize.x/2, 55}, 
                   20, 1, nextSelectionForPlayer1 ? RED : BLUE);
    }
    
    // Draw buttons
    if (CanStartGame()) {
        startButton.Draw();
    }
    
    backButton.Draw();
}