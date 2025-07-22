#include "GameHUD.h"
#include "raylib.h"
#include "ResrcManager.h"

GameHUD::GameHUD(Mario* mario) : mario(mario), luigi(nullptr) {
    // Initialize positions and sizes
    coinsPosition = { 1500, 100 };
    coinsSize = { 50, 50 };
    coinTexture = &ResrcManager::GetInstance().getTexture("GUI_COIN"); // Load your coin texture here

    scorePosition = { 150, 1500 };
    scoreSize = { 200, 50 };
    MarioHUD = &ResrcManager::GetInstance().getTexture("GUI_MARIO"); // Load your HUD texture here

    font = &ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"); // Initialize font if needed
}

GameHUD::GameHUD(Mario* mario, Mario* luigi) : mario(mario), luigi(luigi) {
    // Initialize positions and sizes for multiple players
    coinsPosition = { 10, 10 };
    coinsSize = { 50, 50 };
    coinTexture = nullptr; // Load your coin texture here

    scorePosition = { 10, 70 };
    scoreSize = { 200, 50 };
    MarioHUD = nullptr; // Load your HUD texture here

    font = nullptr; // Initialize font if needed
}

void GameHUD::Draw() {
    int score = mario->GetScore();
    int coins = mario->GetCoins();
    int lives = mario->GetLives();
    DrawTextureNPatch(*coinTexture,
            NPatchInfo{Rectangle{0, 0, (float)(*coinTexture).width,
                                (float)(*coinTexture).height}, 0, 0, 0, 0},
            Rectangle{coinsPosition.x, coinsPosition.y, coinsSize.x, coinsSize.y}, Vector2{0, 0}, 0.0f, WHITE);
    DrawTextEx(*font, ("x"+std::to_string(coins)).c_str(), Vector2{coinsPosition.x + 50, coinsPosition.y }, 20, 2, WHITE);

    /// Draw score
    DrawTextEx(*font, (std::to_string(score)).c_str(), Vector2{scorePosition.x , scorePosition.y +30 }, 20, 2, WHITE);

    //Draw Lives
    DrawTextureNPatch(*MarioHUD,

            NPatchInfo{Rectangle{0, 0, (float)(*MarioHUD).width,
                                (float)(*MarioHUD).height}, 0, 0, 0, 0},
            Rectangle{(float)GetScreenWidth()/2-16-300, scorePosition.y, 120, 24}, Vector2{0, 0}, 0.0f, WHITE);
    DrawTextureNPatch(ResrcManager::GetInstance().getTexture("SMALL_MARIO_0_RIGHT"),
            NPatchInfo{Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("SMALL_MARIO_0_RIGHT").width,
                                24}, 0, 0, 0, 0},
            Rectangle{(float)GetScreenWidth() / 2 -365,scorePosition.y+25 , 32, 24}, Vector2{0, 0}, 0.0f, WHITE);
    DrawTextEx(*font, ("x"+std::to_string(lives)).c_str(), Vector2{(float)GetScreenWidth()/2-20-300, scorePosition.y+30 }, 20, 2, WHITE);
}

void GameHUD::setFont(Font& font) {
    this->font = &font;
}

void GameHUD::setTextColor(Color color) {
    // Set the text color for the HUD
    if (font) {
        DrawTextEx(*font, "Sample Text", Vector2{0, 0}, 20, 2, color);
    }
}

