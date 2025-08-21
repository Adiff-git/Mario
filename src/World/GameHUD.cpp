#include "../inc/World/GameHUD.h"
#include "raylib.h"
#include "ResrcManager.h"

GameHUD::GameHUD(Character *mario) : player1(mario), player2(nullptr)
{

    coinsPosition1 = {1500, 100};
    coinsSize1 = {30, 30};
    coinTexture1 = &ResrcManager::GetInstance().getTexture("GUI_COIN"); // Load your coin texture here

    scorePosition1 = {1500, 120};
    scoreSize1 = {200, 50};
    MarioHUD1 = &ResrcManager::GetInstance().getTexture("GUI_MARIO"); // Load your HUD texture here

    block1 = {1600 / 2 - 20, 100};
    block1Size = {50, 50};
    block1Texture = &ResrcManager::GetInstance().getTexture("GUI_BLOCK"); // Load your block texture here

    Player1StatePos = {1600 / 2 - 10, 100 + 10};
    Player1StateSize = {30, 30};
    Player1StateTexture = nullptr;


//sdasd

    font = &ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"); // Initialize font if needed
}

GameHUD::GameHUD(Character *mario, Character *luigi) : player1(mario), player2(luigi)
{

    coinsPosition1 = {1300 + 110, 110};
    coinsSize1 = {30, 30};
    coinTexture1 = &ResrcManager::GetInstance().getTexture("GUI_COIN"); // Load your coin texture here

    scorePosition1 = {1300 + 110, 120};
    scoreSize1 = {200, 50};
    MarioHUD1 = &ResrcManager::GetInstance().getTexture("GUI_MARIO"); // Load your HUD texture here

    block1 = {1600 / 2 - 20 + 100, 100};
    block1Size = {50, 50};
    block1Texture = &ResrcManager::GetInstance().getTexture("GUI_BLOCK"); // Load your block texture here

    Player1StatePos = {1600 / 2 - 10 + 100, 100 + 10};
    Player1StateSize = {30, 30};
    Player1StateTexture = nullptr;
    //===========================================================================
    coinsPosition2 = {400, 110};
    coinsSize2 = {30, 30};
    coinTexture2 = &ResrcManager::GetInstance().getTexture("GUI_COIN"); // Load your coin texture here

    scorePosition2 = {400, 120};
    scoreSize2 = {200, 50};
    MarioHUD2 = &ResrcManager::GetInstance().getTexture("GUI_MARIO"); // Load your HUD texture here

    block2 = {1600 / 2 - 20 - 100, 100};
    block2Size = {50, 50};
    block2Texture = &ResrcManager::GetInstance().getTexture("GUI_BLOCK"); // Load your block texture here

    Player2StatePos = {1600 / 2 - 10 - 100, 100 + 10};
    Player2StateSize = {30, 30};
    Player2StateTexture = nullptr;

    font = &ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT");
}

void GameHUD::Draw()
{

    char timeStr[16];
    int timeInt = (int)timeLeft;
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d", timeInt / 60, timeInt % 60);

    if (!player2)
    {
        int score = player1->GetScore();
        int coins = player1->GetCoins();
        int lives = player1->GetLives();
        ObjectState marioState = player1->GetMarioState();

        // Draw coins
        DrawTextureNPatch(*coinTexture1,
                          NPatchInfo{Rectangle{0, 0, (float)(*coinTexture1).width,
                                               (float)(*coinTexture1).height},
                                     0, 0, 0, 0},
                          Rectangle{coinsPosition1.x, coinsPosition1.y, coinsSize1.x, coinsSize1.y}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextEx(*font, ("x" + std::to_string(coins)).c_str(), Vector2{coinsPosition1.x + 50, coinsPosition1.y}, 20, 2, WHITE);

        /// Draw score
        DrawTextEx(*font, (std::to_string(score)).c_str(), Vector2{scorePosition1.x, scorePosition1.y + 30}, 20, 2, WHITE);

        // Draw Lives
        DrawTextureNPatch(*MarioHUD1,
                          NPatchInfo{Rectangle{0, 0, (float)(*MarioHUD1).width,
                                               (float)(*MarioHUD1).height},
                                     0, 0, 0, 0},
                          Rectangle{(float)GetScreenWidth() / 2 - 16 - 600, scorePosition1.y, 120, 24}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureNPatch(ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT"),
                          NPatchInfo{Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT").width,
                                               24},
                                     0, 0, 0, 0},
                          Rectangle{(float)GetScreenWidth() / 2 - 630, scorePosition1.y + 25, 32, 24}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextEx(*font, ("x" + std::to_string(lives)).c_str(), Vector2{(float)GetScreenWidth() / 2 - 600, scorePosition1.y + 30}, 20, 2, WHITE);

        // draw BLock
        DrawTextureNPatch(*block1Texture,
                          NPatchInfo{Rectangle{0, 0, (float)(*block1Texture).width,
                                               (float)(*block1Texture).height},
                                     0, 0, 0, 0},
                          Rectangle{block1.x, block1.y, block1Size.x, block1Size.y}, Vector2{0, 0}, 0.0f, WHITE);
        // draw Mario State
        if (marioState == BIG)
        {
            Player1StateTexture = &ResrcManager::GetInstance().getTexture("MUSHROOM_ICON");
            DrawTextureNPatch(*Player1StateTexture,
                              NPatchInfo{Rectangle{0, 0, (float)Player1StateTexture->width,
                                                   (float)Player1StateTexture->height},
                                         0, 0, 0, 0},
                              Rectangle{Player1StatePos.x, Player1StatePos.y, Player1StateSize.x, Player1StateSize.y}, Vector2{0, 0}, 0.0f, WHITE);
        }
        else if (marioState == FIRE)
        {
            Player1StateTexture = &ResrcManager::GetInstance().getTexture("FIREFLOWER_ICON");
            DrawTextureNPatch(*Player1StateTexture,
                              NPatchInfo{Rectangle{0, 0, (float)Player1StateTexture->width,
                                                   (float)Player1StateTexture->height},
                                         0, 0, 0, 0},
                              Rectangle{Player1StatePos.x, Player1StatePos.y, Player1StateSize.x, Player1StateSize.y}, Vector2{0, 0}, 0.0f, WHITE);
        }
    } else {
        int score1 = player1->GetScore();
        int coins1 = player1->GetCoins();
        int lives1 = player1->GetLives();
        ObjectState marioState1 = player1->GetMarioState();

        // Draw Player 1 coins
        DrawTextureNPatch(*coinTexture1,
                          NPatchInfo{Rectangle{0, 0, (float)(*coinTexture1).width,
                                               (float)(*coinTexture1).height},
                                     0, 0, 0, 0},
                          Rectangle{coinsPosition1.x, coinsPosition1.y, coinsSize1.x, coinsSize1.y}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextEx(*font, ("x" + std::to_string(coins1)).c_str(), Vector2{coinsPosition1.x + 50, coinsPosition1.y}, 20, 2, WHITE);

        // Draw Player 1 score
        DrawTextEx(*font, (std::to_string(score1)).c_str(), Vector2{scorePosition1.x, scorePosition1.y + 30}, 20, 2, WHITE);

        // Draw Lives for Player 1
        DrawTextureNPatch(*MarioHUD1,
                          NPatchInfo{Rectangle{0, 0, (float)(*MarioHUD1).width,
                                               (float)(*MarioHUD1).height},
                                     0, 0, 0, 0},
                            Rectangle{(float)GetScreenWidth() / 2 - 16 + 600 - 100, scorePosition1.y, 120, 24}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureNPatch(ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT"),
                          NPatchInfo{Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT").width,
                                               24},
                                     0, 0, 0, 0},
                            Rectangle{(float)GetScreenWidth() / 2 + 600 - 100, scorePosition1.y + 25, 32, 24}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextEx(*font, ("x" + std::to_string(lives1)).c_str(), Vector2{(float)GetScreenWidth() / 2 + 600 + 30 - 100, scorePosition1.y + 30}, 20, 2, WHITE);
        // draw block1
        DrawTextureNPatch(*block1Texture,
            NPatchInfo{Rectangle{0, 0, (float)(*block1Texture).width,
                                 (float)(*block1Texture).height},
                       0, 0, 0, 0},
            Rectangle{block1.x, block1.y, block1Size.x, block1Size.y}, Vector2{0, 0}, 0.0f, WHITE);
        // draw Player 1 State
        if (marioState1 == BIG)
        {
            Player1StateTexture = &ResrcManager::GetInstance().getTexture("MUSHROOM_ICON");
            DrawTextureNPatch(*Player1StateTexture,
                              NPatchInfo{Rectangle{0, 0, (float)Player1StateTexture->width,
                                                   (float)Player1StateTexture->height},
                                         0, 0, 0, 0},
                              Rectangle{Player1StatePos.x, Player1StatePos.y, Player1StateSize.x, Player1StateSize.y}, Vector2{0, 0}, 0.0f, WHITE);
        }
        else if (marioState1 == FIRE)
        {
            Player1StateTexture = &ResrcManager::GetInstance().getTexture("FIREFLOWER_ICON");
            DrawTextureNPatch(*Player1StateTexture,
                              NPatchInfo{Rectangle{0, 0, (float)Player1StateTexture->width,
                                                   (float)Player1StateTexture->height},
                                         0, 0, 0, 0},
                              Rectangle{Player1StatePos.x, Player1StatePos.y, Player1StateSize.x, Player1StateSize.y}, Vector2{0, 0}, 0.0f, WHITE);
        }

        //player2

        // Draw Player 2 coins
        DrawTextureNPatch(*coinTexture2,
            NPatchInfo{Rectangle{0, 0, (float)(*coinTexture2).width,
                                 (float)(*coinTexture2).height},
                       0, 0, 0, 0},
            Rectangle{coinsPosition2.x, coinsPosition2.y, coinsSize2.x, coinsSize2.y}, Vector2{0, 0}, 0.0f, WHITE);
DrawTextEx(*font, ("x" + std::to_string(player2->GetCoins())).c_str(), Vector2{coinsPosition2.x + 50, coinsPosition2.y}, 20, 2, WHITE);


        // Draw Player 2 score
        DrawTextEx(*font, (std::to_string(player2->GetScore())).c_str(), Vector2{scorePosition2.x, scorePosition2.y + 30}, 20, 2, WHITE);


        // Draw Lives for Player 2
        DrawTextureNPatch(*MarioHUD2,
                          NPatchInfo{Rectangle{0, 0, (float)(*MarioHUD2).width,
                                               (float)(*MarioHUD2).height},
                                     0, 0, 0, 0},
                            Rectangle{(float)GetScreenWidth() / 2 - 600 +50 , scorePosition2.y, 120, 24}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextureNPatch(ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT"),
                          NPatchInfo{Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("SMALLMARIO_0_RIGHT").width,
                                               24},
                                     0, 0, 0, 0},
                            Rectangle{(float)GetScreenWidth() / 2 - 630 + 100, scorePosition2.y + 25, 32, 24}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextEx(*font, ("x" + std::to_string(player2->GetLives())).c_str(), Vector2{(float)GetScreenWidth() / 2 - 600 + 100, scorePosition2.y + 30}, 20, 2, WHITE);
        // draw block2
        DrawTextureNPatch(*block2Texture,
            NPatchInfo{Rectangle{0, 0, (float)(*block2Texture).width,
                                 (float)(*block2Texture).height},
                       0, 0, 0, 0},
            Rectangle{block2.x, block2.y, block2Size.x, block2Size.y}, Vector2{0, 0}, 0.0f, WHITE);
        // draw Player 2 State
        ObjectState marioState2 = player2->GetMarioState();
        if (marioState2 == BIG)
        {
            Player2StateTexture = &ResrcManager::GetInstance().getTexture("MUSHROOM_ICON");
            DrawTextureNPatch(*Player2StateTexture,
                              NPatchInfo{Rectangle{0, 0, (float)Player2StateTexture->width,
                                                   (float)Player2StateTexture->height},
                                         0, 0, 0, 0},
                              Rectangle{Player2StatePos.x, Player2StatePos.y, Player2StateSize.x, Player2StateSize.y}, Vector2{0, 0}, 0.0f, WHITE);
        }
        else if (marioState2 == FIRE)
        {
            Player2StateTexture = &ResrcManager::GetInstance().getTexture("FIREFLOWER_ICON");
            DrawTextureNPatch(*Player2StateTexture,
                              NPatchInfo{Rectangle{0, 0, (float)Player2StateTexture->width,
                                                   (float)Player2StateTexture->height},
                                         0, 0, 0, 0},
                              Rectangle{Player2StatePos.x, Player2StatePos.y, Player2StateSize.x, Player2StateSize.y}, Vector2{0, 0}, 0.0f, WHITE);
        }
    }
}

void GameHUD::setFont(Font &font)
{
    this->font = &font;
}

void GameHUD::setTextColor(Color color)
{
    // Set the text color for the HUD
    if (font)
    {
        DrawTextEx(*font, "Sample Text", Vector2{0, 0}, 20, 2, color);
    }
}

void GameHUD::Update(float deltaTime)
{
    if (timeLeft > 0.0f)
    {
        timeLeft -= deltaTime;
    }
    if (timeLeft < 0.0f)
    {
        timeLeft = 0.0f;
    }
}
