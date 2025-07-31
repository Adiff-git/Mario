#include "GameHUD.h"
#include "raylib.h"
#include "ResrcManager.h"

GameHUD::GameHUD(Character* mario) : player1(mario), player2(nullptr) {
    // Initialize positions and sizes
    coinsPosition = { 1500, 100 };
    coinsSize = { 30, 30 };
    coinTexture = &ResrcManager::GetInstance().getTexture("GUI_COIN"); // Load your coin texture here

    scorePosition = { 1500, 120 };
    scoreSize = { 200, 50 };
    MarioHUD = &ResrcManager::GetInstance().getTexture("GUI_MARIO"); // Load your HUD texture here

    block = {1600/2-20, 100};
    blockSize = { 50, 50 };
    blockTexture = &ResrcManager::GetInstance().getTexture("GUI_BLOCK"); // Load your block texture here

    marioStatePosition = { 1600/2 -10, 100 + 10 };
    marioStateSize = { 30, 30 };
    marioStateTexture = nullptr;

    font = &ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"); // Initialize font if needed
}

GameHUD::GameHUD(Character* mario, Character* luigi) : player1(mario), player2(luigi) {
    // Initialize positions and sizes for multiple players
    coinsPosition = { 1500, 100 };
    coinsSize = { 30, 30 };
    coinTexture = &ResrcManager::GetInstance().getTexture("GUI_COIN"); // Load your coin texture here

    scorePosition = { 1500, 120 };
    scoreSize = { 200, 50 };
    MarioHUD = &ResrcManager::GetInstance().getTexture("GUI_MARIO"); // Load your HUD texture here

    block = {1600/2-20, 100};
    blockSize = { 50, 50 };
    blockTexture = &ResrcManager::GetInstance().getTexture("GUI_BLOCK"); // Load your block texture here

    marioStatePosition = { 1600/2 -10, 100 + 10 };
    marioStateSize = { 30, 30 };
    marioStateTexture = nullptr;

    font = &ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT");
}

void GameHUD::Draw() {
    int score = player1->GetScore();
    int coins = player1->GetCoins();
    int lives = player1->GetLives();
    ObjectState marioState = player1->GetMarioState();

    // Draw coins
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
            Rectangle{(float)GetScreenWidth()/2-16-600, scorePosition.y, 120, 24}, Vector2{0, 0}, 0.0f, WHITE);
    DrawTextureNPatch(ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT"),
            NPatchInfo{Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT").width,
                                24}, 0, 0, 0, 0},
            Rectangle{(float)GetScreenWidth() / 2 -630,scorePosition.y+25 , 32, 24}, Vector2{0, 0}, 0.0f, WHITE);
    DrawTextEx(*font, ("x"+std::to_string(lives)).c_str(), Vector2{(float)GetScreenWidth()/2-600, scorePosition.y+30 }, 20, 2, WHITE);

    // draw BLock
    DrawTextureNPatch(*blockTexture,
            NPatchInfo{Rectangle{0, 0, (float)(*blockTexture).width,
                                (float)(*blockTexture).height}, 0, 0, 0, 0},
            Rectangle{block.x, block.y, blockSize.x, blockSize.y}, Vector2{0, 0}, 0.0f, WHITE);
    // draw Mario State
    if ( marioState == BIG) {
        marioStateTexture = &ResrcManager::GetInstance().getTexture("MUSHROOM_ICON");
        DrawTextureNPatch(*marioStateTexture,
                NPatchInfo{Rectangle{0, 0, (float)marioStateTexture->width,
                                    (float)marioStateTexture->height}, 0, 0, 0, 0},
                Rectangle{marioStatePosition.x, marioStatePosition.y, marioStateSize.x, marioStateSize.y}, Vector2{0, 0}, 0.0f, WHITE);
    } else if (marioState == FIRE) {
        marioStateTexture = &ResrcManager::GetInstance().getTexture("FIREFLOWER_ICON");
        DrawTextureNPatch(*marioStateTexture,
                NPatchInfo{Rectangle{0, 0, (float)marioStateTexture->width,
                                    (float)marioStateTexture->height}, 0, 0, 0, 0},
                Rectangle{marioStatePosition.x, marioStatePosition.y, marioStateSize.x, marioStateSize.y}, Vector2{0, 0}, 0.0f, WHITE);
    } 
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

