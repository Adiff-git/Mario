#include "../inc/Screen/GameScreen.h"
#include "../inc/Screen/MenuScreen.h"
#include "../inc/Screen/MapSelectScreen.h"
#include "SoundManager.h"
#include <raylib.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

GameScreen::GameScreen(ScreenController* screenController)
    : Screen(screenController), 
      BackMenu(Vector2{50, 50}, Vector2{50, 50}), 
      level(5), 
      transitionState(TransitionState::NONE), 
      transitionTime(1.0f), 
      transitionTimeAcum(0.0f),
      gameHUD(nullptr),
      isMultiplayer(false),
      player1Type(CharacterType::MARIO),
      player2Type(CharacterType::LUIGI),
      pauseMusicVolume(0),
      pauseSfxVolume(0)
{
    gameWorld = std::make_unique<GameWorld>(level, this);
    gameWorld->player1->SetLives(3);
    gameWorld->player1->SetCoins(0);
    gameWorld->player1->SetScore(0);
    gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
    BackMenu.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));  

    // Lấy giá trị âm lượng từ SoundManager (đồng bộ với SettingsScreen)
    pauseMusicVolume = (int)(SoundManager::GetInstance().GetMusicVol("MENU") * 100.0f);
    pauseSfxVolume = (int)(SoundManager::GetInstance().GetSoundVol("COIN_COLLECTION") * 100.0f);

    SoundManager::GetInstance().StopAllSounds();
    string musicKey = "GAMEWORLD_" + std::to_string(level);
    SoundManager::GetInstance().SetMusicVol(musicKey, pauseMusicVolume / 100.0f);
    SoundManager::GetInstance().PlayMusic(musicKey);
}

GameScreen::GameScreen(ScreenController* screenController, bool multiplayer, 
    CharacterType p1Type, CharacterType p2Type)
    : Screen(screenController), 
      BackMenu(Vector2{50, 50}, Vector2{50, 50}), 
      level(5), 
      transitionState(TransitionState::NONE), 
      transitionTime(1.0f), 
      transitionTimeAcum(0.0f),
      gameHUD(nullptr),
      isMultiplayer(multiplayer),
      player1Type(p1Type),
      player2Type(p2Type),
      pauseMusicVolume(0),
      pauseSfxVolume(0)
{
    gameWorld = std::make_unique<GameWorld>(level, this, multiplayer, p1Type, p2Type);

    gameWorld->player1->SetLives(3);
    gameWorld->player1->SetCoins(0);
    gameWorld->player1->SetScore(0);

    if (multiplayer && gameWorld->player2) {
        gameWorld->player2->SetLives(3);
        gameWorld->player2->SetCoins(0);
        gameWorld->player2->SetScore(0);
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    } else {
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
    }

    BackMenu.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));  

    // Lấy giá trị âm lượng từ SoundManager (đồng bộ với SettingsScreen)
    pauseMusicVolume = (int)(SoundManager::GetInstance().GetMusicVol("MENU") * 100.0f);
    pauseSfxVolume = (int)(SoundManager::GetInstance().GetSoundVol("COIN_COLLECTION") * 100.0f);

    SoundManager::GetInstance().StopAllSounds();
    string musicKey = "GAMEWORLD_" + std::to_string(level);
    SoundManager::GetInstance().SetMusicVol(musicKey, pauseMusicVolume / 100.0f);
    SoundManager::GetInstance().PlayMusic(musicKey);
}

GameScreen::GameScreen(ScreenController* screenController, bool multiplayer, 
    CharacterType p1Type, CharacterType p2Type, MapType map, DifficultyLevel difficulty)
        : Screen(screenController), 
        BackMenu(Vector2{50, 50}, Vector2{50, 50}), 
        level(1), 
        transitionState(TransitionState::NONE), 
        transitionTime(1.0f), 
        transitionTimeAcum(0.0f),
        gameHUD(nullptr),
        isMultiplayer(multiplayer),
        player1Type(p1Type),
        player2Type(p2Type),
        selectedMap(map),
        selectedDifficulty(difficulty),
        pauseMusicVolume(0),
        pauseSfxVolume(0)
{
    // Adjust level based on map selection
    switch(map) {
        case MapType::MAP_TUTORIAL: level = 0; break;
        case MapType::MAP_1: level = 1; break;
        case MapType::MAP_2: level = 2; break;
        case MapType::MAP_3: level = 3; break;
        case MapType::MAP_BOSS: level = 4; break;
        default: level = 1; break;
    }

    // Calculate enemy speed multiplier based on both map level and difficulty
    float baseSpeedMultiplier = 1.0f;
    float levelMultiplier = 1.0f;
    
    // Base multiplier from difficulty
    switch(difficulty) {
        case DifficultyLevel::EASY: baseSpeedMultiplier = 0.6f; break;
        case DifficultyLevel::MEDIUM: baseSpeedMultiplier = 1.0f; break;
        case DifficultyLevel::HARD: baseSpeedMultiplier = 1.8f; break;
        default: baseSpeedMultiplier = 1.0f; break;
    }
    
    // Additional multiplier from map level
    switch(level) {
        case 1: levelMultiplier = 1.0f; break;   
        case 2: levelMultiplier = 2.0f; break;   
        case 3: levelMultiplier = 4.0f; break;   
        default: levelMultiplier = 1.0f; break;
    }
    
    // Combine both multipliers
    float enemySpeedMultiplier = baseSpeedMultiplier * levelMultiplier;

    gameWorld = std::make_unique<GameWorld>(level, this, multiplayer, p1Type, p2Type, enemySpeedMultiplier);

    // Initialize Player 1
    int initialLives = 3;
    gameWorld->player1->SetLives(initialLives);
    gameWorld->player1->SetCoins(0);
    gameWorld->player1->SetScore(0);

    // Initialize Player 2 if multiplayer
    if (multiplayer && gameWorld->player2) {
        gameWorld->player2->SetLives(initialLives);
        gameWorld->player2->SetCoins(0);
        gameWorld->player2->SetScore(0);
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    } else {
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
    }

    BackMenu.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));  

    // Lấy giá trị âm lượng từ SoundManager
    pauseMusicVolume = (int)(SoundManager::GetInstance().GetMusicVol("MENU") * 100.0f);
    pauseSfxVolume = (int)(SoundManager::GetInstance().GetSoundVol("COIN_COLLECTION") * 100.0f);

    SoundManager::GetInstance().StopAllSounds();
    string musicKey = "GAMEWORLD_" + std::to_string(level);
    SoundManager::GetInstance().SetMusicVol(musicKey, pauseMusicVolume / 100.0f);
    SoundManager::GetInstance().PlayMusic(musicKey);
}

void GameScreen::Update() {
    // Kiểm tra nút BackMenu trước để đảm bảo nó hoạt động ở mọi trạng thái
    BackMenu.Update();
    if (BackMenu.IsPressed()) {
        if (!SoundManager::GetInstance().IsMuted()) {
            SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        }
        screenController->ChangeScreen(new MenuScreen(screenController));
        return;
    }

    if (requestGoHome) {
        if (!SoundManager::GetInstance().IsMuted()) {
            SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        }
        screenController->ChangeScreen(new MenuScreen(screenController));
        return;
    }

    // Toggle pause/resume bằng phím P
    if (IsKeyPressed(KEY_P)) {
        isPaused = !isPaused;
        showPauseMenu = isPaused;
    }

    if (showPauseMenu) {
        // Cửa sổ pause
        int winW = 400, winH = 350;
        int winX = GetScreenWidth()/2 - winW/2;
        int winY = GetScreenHeight()/2 - winH/2;
        int sliderW = 200;
        int sliderX = winX + 100;
        int sliderY = winY + 80;

        // --- MUSIC SLIDER ---
        Rectangle musicSliderRect = { (float)sliderX, (float)sliderY - 10, (float)sliderW, 30 };
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), musicSliderRect)) {
            // Tự động unmute khi kéo thanh trượt
            if (SoundManager::GetInstance().IsMuted()) {
                SoundManager::GetInstance().SetMuted(false);
                std::cout << "[GameScreen] Unmuted by Music slider" << std::endl;
            }
            float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
            percent = std::max(0.0f, std::min(1.0f, percent));
            pauseMusicVolume = (int)(percent * 100);
            string musicKey = "GAMEWORLD_" + std::to_string(level);
            SoundManager::GetInstance().SetMusicVol("MENU", pauseMusicVolume / 100.0f);
            SoundManager::GetInstance().SetMusicVol(musicKey, pauseMusicVolume / 100.0f);
            std::cout << "[GameScreen] Music volume set to: " << pauseMusicVolume << std::endl;
        }

        // --- SFX SLIDER ---
        int sfxSliderY = sliderY + 70;
        Rectangle sfxSliderRect = { (float)sliderX, (float)sfxSliderY - 10, (float)sliderW, 30 };
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), sfxSliderRect)) {
            // Tự động unmute khi kéo thanh trượt
            if (SoundManager::GetInstance().IsMuted()) {
                SoundManager::GetInstance().SetMuted(false);
                std::cout << "[GameScreen] Unmuted by SFX slider" << std::endl;
            }
            float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
            percent = std::max(0.0f, std::min(1.0f, percent));
            pauseSfxVolume = (int)(percent * 100);
            SoundManager::GetInstance().SetSoundVol("COIN", pauseSfxVolume / 100.0f);
            SoundManager::GetInstance().SetSoundVol("COIN_COLLECTION", pauseSfxVolume / 100.0f);
            SoundManager::GetInstance().SetSoundVol("POWER_UP_APPEARS", pauseSfxVolume / 100.0f);
            SoundManager::GetInstance().SetSoundVol("ENEMY_DEATH", pauseSfxVolume / 100.0f);
            SoundManager::GetInstance().SetSoundVol("MARIO_JUMP", pauseSfxVolume / 100.0f);
            SoundManager::GetInstance().SetSoundVol("BUTTON_CLICK", pauseSfxVolume / 100.0f);
            std::cout << "[GameScreen] SFX volume set to: " << pauseSfxVolume << std::endl;
        }

        // --- Nút Resume và Home ---
        Rectangle resumeBtn = { (float)(winX + 50), (float)(winY + winH - 80), 100, 50 };
        Rectangle homeBtn = { (float)(winX + 250), (float)(winY + winH - 80), 100, 50 };
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, resumeBtn)) {
                if (!SoundManager::GetInstance().IsMuted()) {
                    SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                }
                isPaused = false;
                showPauseMenu = false;
            }
            if (CheckCollisionPointRec(mouse, homeBtn)) {
                if (!SoundManager::GetInstance().IsMuted()) {
                    SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                }
                requestGoHome = true;
            }
        }

        // Debug log
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            std::cout << "[GameScreen Debug] Mouse pos: (" << mousePos.x << ", " << mousePos.y 
                      << "), sfxSliderRect: (" << sfxSliderRect.x << ", " << sfxSliderRect.y 
                      << ", " << sfxSliderRect.width << ", " << sfxSliderRect.height << ")" 
                      << ", collision: " << CheckCollisionPointRec(GetMousePosition(), sfxSliderRect) << std::endl;
        }
        return;
    }

    if (showWinScreen) {
        if (IsKeyPressed(KEY_ENTER)) {
            if (!SoundManager::GetInstance().IsMuted()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            showWinScreen = false;
            screenController->ChangeScreen(new MenuScreen(screenController));
        }
        return;
    }

    switch (transitionState) {
        case TransitionState::NEXT_LEVEL:
            transitionTimeAcum += GameClock::GetInstance().FIXED_TIME_STEP;
            if (transitionTimeAcum >= transitionTime) {
                BeginTransition(TransitionState::NONE);
            }
            break;
        case TransitionState::GAME_OVER:
            transitionTimeAcum += GameClock::GetInstance().FIXED_TIME_STEP;
            if (transitionTimeAcum >= transitionTime) {
                BeginTransition(TransitionState::NONE);
            }
            break;
        case TransitionState::GAME_RESET:
            transitionTimeAcum += GameClock::GetInstance().FIXED_TIME_STEP;
            if (transitionTimeAcum >= transitionTime) {
                BeginTransition(TransitionState::NONE);
            }
            break;
        default:
            break;
    }

    if (transitionState != TransitionState::NONE) {
        return;
    }

    switch (gameWorld->GetGameState()) {
        case GameState::GAME_PLAYING:
            gameWorld->UpdateWorld();
            break;
        case GameState::GAME_COMPLETED:
            if (IsKeyPressed(KEY_ENTER)) {
                if (!SoundManager::GetInstance().IsMuted()) {
                    SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                }
                NextLevel();
                BeginTransition(TransitionState::NEXT_LEVEL);
            }
            break;
        case GameState::GAME_RESET:
            if (IsKeyPressed(KEY_ENTER)) {
                if (!SoundManager::GetInstance().IsMuted()) {
                    SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                }
                ResetGame();
                BeginTransition(TransitionState::GAME_RESET);
            }
            break;
        case GameState::GAME_OVER:
            static bool gameOverMusicPlayed = false;
            if (!gameOverMusicPlayed) {
                SoundManager::GetInstance().StopAllSounds();
                SoundManager::GetInstance().PlayMusic("GAME_OVER");
                gameOverMusicPlayed = true;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                if (!SoundManager::GetInstance().IsMuted()) {
                    SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                }
                gameOverMusicPlayed = false;
                ResetGame();
                BeginTransition(TransitionState::GAME_OVER);
            }
            break;
        default:
            break;
    }
}

void GameScreen::Draw() {
    if (showWinScreen) {
        Texture2D& winTex = ResrcManager::GetInstance().getTexture("WIN");
        DrawTexturePro(
            winTex,
            { 0, 0, (float)winTex.width, (float)winTex.height },
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            { 0, 0 },
            0.0f,
            WHITE
        );
        DrawText("Press ENTER to return to menu", GetScreenWidth()/2-200, GetScreenHeight()-100, 32, BLACK);
        return;
    }

    gameWorld->DrawWorld();
    gameHUD->Draw();
    if (gameWorld->IsCompleted()) {
        if (transitionState == TransitionState::NONE) {
            DrawEnd();
        } 
    }

    // Pause menu
    if (showPauseMenu) {
        // Overlay mờ
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));

        // Cửa sổ pause
        int winW = 400, winH = 350;
        int winX = GetScreenWidth()/2 - winW/2;
        int winY = GetScreenHeight()/2 - winH/2;
        DrawRectangleRounded({(float)winX, (float)winY, (float)winW, (float)winH}, 0.2f, 10, WHITE);
        DrawText("PAUSED", winX + 140, winY + 20, 32, BLACK);

        // --- MUSIC SLIDER ---
        int sliderW = 200;
        int sliderX = winX + 100;
        int sliderY = winY + 80;
        DrawText("Music Volume", sliderX, sliderY - 30, 20, DARKGRAY);
        DrawRectangle(sliderX, sliderY, sliderW, 10, LIGHTGRAY);
        DrawRectangle(sliderX, sliderY, (int)(pauseMusicVolume * (sliderW/100.0f)), 10, BLUE);
        int knobX = sliderX + (int)(pauseMusicVolume * (sliderW/100.0f));
        DrawCircle(knobX, sliderY + 5, 10, DARKBLUE);
        DrawText(TextFormat("%d%%", pauseMusicVolume), sliderX + sliderW + 20, sliderY - 10, 20, BLACK);

        // --- SFX SLIDER ---
        int sfxSliderY = sliderY + 70;
        DrawText("SFX Volume", sliderX, sfxSliderY - 30, 20, DARKGRAY);
        DrawRectangle(sliderX, sfxSliderY, sliderW, 10, LIGHTGRAY);
        DrawRectangle(sliderX, sfxSliderY, (int)(pauseSfxVolume * (sliderW/100.0f)), 10, ORANGE);
        int sfxKnobX = sliderX + (int)(pauseSfxVolume * (sliderW/100.0f));
        DrawCircle(sfxKnobX, sfxSliderY + 5, 10, DARKGRAY);
        DrawText(TextFormat("%d%%", pauseSfxVolume), sliderX + sliderW + 20, sfxSliderY - 10, 20, BLACK);

        // --- Nút Resume ---
        Rectangle resumeBtn = { (float)(winX + 50), (float)(winY + winH - 80), 100, 50 };
        DrawRectangleRec(resumeBtn, GREEN);
        DrawText("Play", resumeBtn.x + 25, resumeBtn.y + 15, 24, WHITE);

        // --- Nút Home ---
        Rectangle homeBtn = { (float)(winX + 250), (float)(winY + winH - 80), 100, 50 };
        DrawRectangleRec(homeBtn, RED);
        DrawText("Home", homeBtn.x + 20, homeBtn.y + 15, 24, WHITE);
    }

    // Game over and reset screens
    Texture *GameOver = &ResrcManager::GetInstance().getTexture("GAME_OVER");
    Font* SuperMarioFont = &ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT");
    Texture *SmallMario = &ResrcManager::GetInstance().getTexture("SMALL_MARIO_0_RIGHT");
    if (gameWorld->GetGameState() == GameState::GAME_OVER && transitionState == TransitionState::NONE) {
        DrawRectangle(0,0, GetScreenWidth(), GetScreenHeight(), BLACK);
        DrawTextureNPatch(*GameOver,NPatchInfo{Rectangle{0, 0, (float)(*GameOver).width,(float)(*GameOver).height}, 0, 0, 0, 0},Rectangle{(float)GetScreenWidth() / 2 - 320, (float)GetScreenHeight() / 2 - 150, 640, 128}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextEx(*SuperMarioFont, "Press ENTER to restart", Vector2{(float)GetScreenWidth() / 2 - 200, (float)GetScreenHeight() / 2 + 100}, 20, 7, WHITE);
    }
    if (gameWorld->GetGameState() == GameState::GAME_RESET && transitionState == TransitionState::NONE) {
        DrawTextEx(*SuperMarioFont, 
            "Press ENTER to restart",
             Vector2{(float)GetScreenWidth() / 2 - 200, 
                (float)GetScreenHeight() / 2 + 100}, 20, 7, WHITE);
    }

    switch(transitionState) {
        case TransitionState::NEXT_LEVEL:
            {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
            DrawTextEx(*SuperMarioFont, ("Level " + std::to_string(level )).c_str(),
                Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(
                *SuperMarioFont,
                ("Level " + std::to_string(level )).c_str(), 20,7).x/2,
                (float)GetScreenHeight() / 2 -100}, 
                20, 7, WHITE);
            DrawTextureNPatch(*SmallMario,
                NPatchInfo{Rectangle{0, 0, (float)(*SmallMario).width, 24}, 0, 0, 0, 0},
                Rectangle{(float)GetScreenWidth() / 2 -100, (float)GetScreenHeight() / 2-16, 43, 32}, Vector2{0, 0}, 0.0f, WHITE);
                
                Vector2 size = MeasureTextEx(*SuperMarioFont,
                ("X " + std::to_string(level + 1)).c_str(), 20, 7);
            DrawTextEx(*SuperMarioFont, ("X " + std::to_string(gameWorld->player1->GetLives())).c_str(),
                Vector2{(float)GetScreenWidth() / 2 -size.x/2,
                    (float)GetScreenHeight() / 2-size.y/2 }, 
                20, 7, WHITE);
            break;
            }
        case TransitionState::GAME_RESET:
            {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
            
            DrawTextEx(*SuperMarioFont, ("Level " + std::to_string(level )).c_str(),
            Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(
            *SuperMarioFont,
            ("Level " + std::to_string(level )).c_str(),
            20,7).x/2,
            (float)GetScreenHeight() / 2 -100}, 
            20, 7, WHITE);

            DrawTextureNPatch(*SmallMario,
                        NPatchInfo{Rectangle{0, 0, (float)(*SmallMario).width,
                                            24}, 0, 0, 0, 0},
                        Rectangle{(float)GetScreenWidth() / 2 -100, (float)GetScreenHeight() / 2-16, 43, 32}, Vector2{0, 0}, 0.0f, WHITE);
                        
            Vector2 size = MeasureTextEx(*SuperMarioFont,
                ("X " + std::to_string(level + 1)).c_str(), 20, 7);

            DrawTextEx(*SuperMarioFont, ("X " + std::to_string(gameWorld->player1->GetLives())).c_str(),
            Vector2{(float)GetScreenWidth() / 2 -size.x/2,
            (float)GetScreenHeight() / 2-size.y/2 }, 
            20, 7, WHITE);
            break;
            }
        case TransitionState::GAME_OVER:
            {
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
            
            DrawTextEx(*SuperMarioFont, ("Level " + std::to_string(level )).c_str(),
            Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(
            *SuperMarioFont,
            ("Level " + std::to_string(level )).c_str(),
            20,7).x/2,
            (float)GetScreenHeight() / 2 -100}, 
            20, 7, WHITE);

            DrawTextureNPatch(*SmallMario,
                        NPatchInfo{Rectangle{0, 0, (float)(*SmallMario).width,
                                            24}, 0, 0, 0, 0},
                        Rectangle{(float)GetScreenWidth() / 2 -100, (float)GetScreenHeight() / 2-16, 43, 32}, Vector2{0, 0}, 0.0f, WHITE);
                        
            Vector2 size = MeasureTextEx(*SuperMarioFont,
                ("X " + std::to_string(level + 1)).c_str(), 20, 7);

            DrawTextEx(*SuperMarioFont, ("X " + std::to_string(gameWorld->player1->GetLives())).c_str(),
            Vector2{(float)GetScreenWidth() / 2 -size.x/2,
            (float)GetScreenHeight() / 2-size.y/2 }, 
            20, 7, WHITE);
            break;
            }
    }
    BackMenu.Draw();
}

void GameScreen::ResetGame() {
    // Lưu thông tin trước khi xóa gameWorld
    int currentLives1 = gameWorld->player1->GetLives();
    int currentLives2 = gameWorld->player2 ? gameWorld->player2->GetLives() : 0;
    if (currentLives1 > 1 || (isMultiplayer && currentLives2 > 0)) {
        gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);
         
        gameWorld->player1->SetLives(currentLives1 - 1);
        if (isMultiplayer && gameWorld->player2) {
            gameWorld->player2->SetLives(currentLives2  -1);
        }
    } else {
        level = 1;
        gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);
        gameWorld->player1->SetLives(3);
        gameWorld->player1->SetCoins(0);
        gameWorld->player1->SetScore(0);
        if (isMultiplayer && gameWorld->player2) {
            gameWorld->player2->SetLives(3);
            gameWorld->player2->SetCoins(0);
            gameWorld->player2->SetScore(0);
        }
    }
    if (isMultiplayer) {
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    } else {
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
    }
}

void GameScreen::DrawEnd() {
    // Vẽ texture WIN (Course Cleared.png)
    Texture2D& winTex = ResrcManager::GetInstance().getTexture("WIN");
    DrawTexturePro(
        winTex,
        {0, 0, (float)winTex.width, (float)winTex.height},
        {(float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 300, 800, 600},
        {0, 0}, 0.0f, WHITE);

    Rectangle backBtn = {(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 250, 200, 60};
    DrawRectangleRounded(backBtn, 0.3f, 20, Color{234,136,65,255});
    DrawText("Back to Menu", (int)backBtn.x + 20, (int)backBtn.y + 15, 32, WHITE);

    bool backPressed = false;
    // if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    //     Vector2 mouse = GetMousePosition();
    //     if (mouse.x >= backBtn.x && mouse.x <= backBtn.x + backBtn.width &&
    //         mouse.y >= backBtn.y && mouse.y <= backBtn.y + backBtn.height) {
    //         backPressed = true;
    //     }
    // }
    if (IsKeyPressed(KEY_ENTER)) {
        backPressed = true;
    }
    if (backPressed) {
        screenController->ChangeScreen(new MenuScreen(screenController));
    }
    
    
}

void GameScreen::NextLevel() {
    int currentLives1 = gameWorld->player1->GetLives();
    int currentCoins1 = gameWorld->player1->GetCoins();
    int currentScore1 = gameWorld->player1->GetScore();
    ObjectState currentPlayerState1 = gameWorld->player1->GetMarioState();

    int currentLives2 = isMultiplayer && gameWorld->player2 ? gameWorld->player2->GetLives() : 0;
    int currentCoins2 = isMultiplayer && gameWorld->player2 ? gameWorld->player2->GetCoins() : 0;
    int currentScore2 = isMultiplayer && gameWorld->player2 ? gameWorld->player2->GetScore() : 0;
    ObjectState currentPlayerState2 = SMALL;
    if (isMultiplayer && gameWorld->player2) {
        currentPlayerState2 =  gameWorld->player2->GetMarioState();
    }
    
    MapType nextMap = selectedMap;
    DifficultyLevel nextDifficulty = selectedDifficulty;

    if (selectedMap == MapType::MAP_TUTORIAL) {
        nextMap = MapType::MAP_1;
        nextDifficulty = selectedDifficulty;
    } else if (selectedMap == MapType::MAP_1) {
        nextMap = MapType::MAP_2;
        nextDifficulty = selectedDifficulty;
    } else if (selectedMap == MapType::MAP_2) {
        nextMap = MapType::MAP_3;
        nextDifficulty = selectedDifficulty;
    } else if (selectedMap == MapType::MAP_3) {
        if (selectedDifficulty == DifficultyLevel::EASY) {
            nextMap = MapType::MAP_3;
            nextDifficulty = DifficultyLevel::MEDIUM;
        } else if (selectedDifficulty == DifficultyLevel::MEDIUM) {
            nextMap = MapType::MAP_3;
            nextDifficulty = DifficultyLevel::HARD;
        } else {
            nextMap = MapType::MAP_BOSS;
            nextDifficulty = DifficultyLevel::EASY;
        }
    } else if (selectedMap == MapType::MAP_BOSS) {
        screenController->ChangeScreen(new MenuScreen(screenController));
        return;
    }

    int nextLevel = 1;
    switch (nextMap) {
        case MapType::MAP_TUTORIAL: nextLevel = 0; break;
        case MapType::MAP_1: nextLevel = 1; break;
        case MapType::MAP_2: nextLevel = 2; break;
        case MapType::MAP_3: nextLevel = 3; break;
        case MapType::MAP_BOSS: nextLevel = 4; break;
        default: nextLevel = 1; break;
    }
    selectedMap = nextMap;
    selectedDifficulty = nextDifficulty;
    level = nextLevel;
    gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);
    
    gameWorld->player1->SetLives(currentLives1);
    gameWorld->player1->SetCoins(currentCoins1);
    gameWorld->player1->SetScore(currentScore1);
    gameWorld->player1->SetMarioState(currentPlayerState1);
    if (isMultiplayer && gameWorld->player2) {
        gameWorld->player2->SetLives(currentLives2);
        gameWorld->player2->SetCoins(currentCoins2);
        gameWorld->player2->SetScore(currentScore2);
        gameWorld->player2->SetMarioState(currentPlayerState2);
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    } else gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
}

void GameScreen::BeginTransition(TransitionState transitionState) {
    this->transitionState = transitionState;
    transitionTimeAcum = 0.0f;
    switch (transitionState) {
        case TransitionState::NEXT_LEVEL:
            transitionTime = 3;
            SoundManager::GetInstance().StopAllSounds();
            break;
        case TransitionState::GAME_OVER:
            transitionTime = 3;
            SoundManager::GetInstance().StopAllSounds();
            break;
        case TransitionState::GAME_RESET:
            transitionTime = 3;
            SoundManager::GetInstance().StopAllSounds();
            break;
        case TransitionState::NONE:
            transitionTime = 3;
            SoundManager::GetInstance().StopAllSounds();
            string musicKey = "GAMEWORLD_" + std::to_string(level);
            SoundManager::GetInstance().SetMusicVol(musicKey, pauseMusicVolume / 100.0f);
            SoundManager::GetInstance().PlayMusic(musicKey);
            break;
    }
}

void GameScreen::SaveCurrentGame(int slotIndex) {
    GameSaveData saveData;

    saveData.saveDateTime = GetCurrentDateTime();
    saveData.currentLevel = level;
    saveData.isMultiplayer = isMultiplayer;

    saveData.player1.characterType = static_cast<int>(player1Type);
    saveData.player1.lives = gameWorld->player1->GetLives();
    saveData.player1.coins = gameWorld->player1->GetCoins();
    saveData.player1.score = gameWorld->player1->GetScore();
    saveData.player1.x = gameWorld->player1->GetPos().x;
    saveData.player1.y = gameWorld->player1->GetPos().y;
    saveData.player1.velX = gameWorld->player1->GetVel().x;
    saveData.player1.velY = gameWorld->player1->GetVel().y;
    saveData.player1.playerState = static_cast<int>(gameWorld->player1->GetMarioState());
    saveData.player1.additionalState = static_cast<int>(gameWorld->player1->GetAdditionalState());

    if (isMultiplayer && gameWorld->player2) {
        saveData.player2.characterType = static_cast<int>(player2Type);
        saveData.player2.lives = gameWorld->player2->GetLives();
        saveData.player2.coins = gameWorld->player2->GetCoins();
        saveData.player2.score = gameWorld->player2->GetScore();
        saveData.player2.x = gameWorld->player2->GetPos().x;
        saveData.player2.y = gameWorld->player2->GetPos().y;
        saveData.player2.velX = gameWorld->player2->GetVel().x;
        saveData.player2.velY = gameWorld->player2->GetVel().y;
        saveData.player2.playerState = static_cast<int>(gameWorld->player2->GetMarioState());
        saveData.player2.additionalState = static_cast<int>(gameWorld->player2->GetAdditionalState());
    }

    gameWorld->CollectWorldData(saveData);
    if (SaveManager::GetInstance().SaveGame(saveData, slotIndex)) {
        std::cout << "Game saved successfully to slot " << slotIndex << std::endl;
    } else {
        std::cerr << "Failed to save game to slot " << slotIndex << std::endl;
    }
}

void GameScreen::LoadSavedGame(int slotIndex) {
    GameSaveData saveData;
    if (SaveManager::GetInstance().LoadGame(saveData, slotIndex)) {
        std::cout << "Game loaded successfully from slot " << slotIndex << std::endl;

        level = saveData.currentLevel;
        isMultiplayer = saveData.isMultiplayer;
        
        gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, 
                                                static_cast<CharacterType>(saveData.player1.characterType),
                                                static_cast<CharacterType>(saveData.player2.characterType));

        gameWorld->player1->SetLives(saveData.player1.lives);
        gameWorld->player1->SetCoins(saveData.player1.coins);
        gameWorld->player1->SetScore(saveData.player1.score);
        gameWorld->player1->SetPos({saveData.player1.x, saveData.player1.y});
        gameWorld->player1->SetVel({saveData.player1.velX, saveData.player1.velY});
        gameWorld->player1->SetMarioState(static_cast<ObjectState>(saveData.player1.playerState));
        gameWorld->player1->SetAdditionalState(static_cast<ObjectState>(saveData.player1.additionalState));

        if (isMultiplayer && gameWorld->player2) {
            gameWorld->player2->SetLives(saveData.player2.lives);
            gameWorld->player2->SetCoins(saveData.player2.coins);
            gameWorld->player2->SetScore(saveData.player2.score);
            gameWorld->player2->SetPos({saveData.player2.x, saveData.player2.y});
            gameWorld->player2->SetVel({saveData.player2.velX, saveData.player2.velY});
            gameWorld->player2->SetMarioState(static_cast<ObjectState>(saveData.player2.playerState));
            gameWorld->player2->SetAdditionalState(static_cast<ObjectState>(saveData.player2.additionalState));
            gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
        } else {
            gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
        }
    } else {
        std::cerr << "Failed to load game from slot " << slotIndex << std::endl;
    }
}

std::string GameScreen::GetCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void GameScreen::HandleSaveLoadInput() {
    if (IsKeyPressed(KEY_U)) {
        SaveCurrentGame(1);
    }
    if (IsKeyPressed(KEY_I)) {
        LoadSavedGame(1);
    }
}