#include "../inc/Screen/GameScreen.h"
#include "../inc/Screen/MenuScreen.h"
#include "SoundManager.h"
GameScreen::GameScreen(ScreenController* screenController)
    : Screen(screenController), 
      BackMenu(Vector2{50, 50}, Vector2{50, 50}), 
      level(3), 
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

void GameScreen::Update() {
    BackMenu.Update();
    if (requestGoHome) {
        screenController->ChangeScreen(new MenuScreen(screenController));
        return;
    }
    // Toggle pause/resume bằng phím P
    if (IsKeyPressed(KEY_P)) {
        isPaused = !isPaused;
        showPauseMenu = isPaused;
    }

    if (showPauseMenu) {
    // Overlay mờ
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));

    // Cửa sổ pause
    int winW = 400, winH = 400;
    int winX = GetScreenWidth()/2 - winW/2;
    int winY = GetScreenHeight()/2 - winH/2;
    DrawRectangleRounded({(float)winX, (float)winY, (float)winW, (float)winH}, 0.2f, 10, WHITE);
    DrawText("PAUSED", winX + 140, winY + 20, 32, BLACK);

    // --- MUSIC SLIDER ---
    int sliderW = 200;
    int sliderX = winX + 100;
    int sliderY = winY + 80;
    // Cập nhật giá trị từ SoundManager nếu không mute
    if (!SoundManager::GetInstance().IsMuted()) {
        pauseMusicVolume = (int)(SoundManager::GetInstance().GetMusicVol("MENU") * 100.0f);
    }
    DrawText("Music Volume", sliderX, sliderY - 30, 20, DARKGRAY);
    DrawRectangle(sliderX, sliderY, sliderW, 10, LIGHTGRAY);
    DrawRectangle(sliderX, sliderY, (int)(pauseMusicVolume * (sliderW/100.0f)), 10, BLUE);
    int knobX = sliderX + (int)(pauseMusicVolume * (sliderW/100.0f));
    DrawCircle(knobX, sliderY + 5, 10, DARKBLUE);

    Rectangle musicSliderRect = { (float)sliderX, (float)sliderY - 10, (float)sliderW, 30 };
    if (!SoundManager::GetInstance().IsMuted() && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), musicSliderRect)) {
        float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
        percent = std::max(0.0f, std::min(1.0f, percent));
        pauseMusicVolume = (int)(percent * 100);
        // Cập nhật tất cả music trong SoundManager
        SoundManager::GetInstance().SetMusicVol("MENU", pauseMusicVolume / 100.0f);
        SoundManager::GetInstance().SetMusicVol("GAMEWORLD_1", pauseMusicVolume / 100.0f);
        SoundManager::GetInstance().SetMusicVol("GAMEWORLD_2", pauseMusicVolume / 100.0f);
        SoundManager::GetInstance().SetMusicVol("GAMEWORLD_3", pauseMusicVolume / 100.0f);
        SoundManager::GetInstance().SetMusicVol("GAMEWORLD_4", pauseMusicVolume / 100.0f);
    }
    DrawText(TextFormat("%d", pauseMusicVolume), sliderX + sliderW + 20, sliderY - 10, 20, BLACK);

    // --- SFX SLIDER ---
    int sfxSliderY = sliderY + 70;
    // Cập nhật giá trị từ SoundManager nếu không mute
    if (!SoundManager::GetInstance().IsMuted()) {
        pauseSfxVolume = (int)(SoundManager::GetInstance().GetSoundVol("COIN_COLLECTION") * 100.0f);
    }
    DrawText("SFX Volume", sliderX, sfxSliderY - 30, 20, DARKGRAY);
    DrawRectangle(sliderX, sfxSliderY, sliderW, 10, LIGHTGRAY);
    DrawRectangle(sliderX, sfxSliderY, (int)(pauseSfxVolume * (sliderW/100.0f)), 10, ORANGE);
    int sfxKnobX = sliderX + (int)(pauseSfxVolume * (sliderW/100.0f));
    DrawCircle(sfxKnobX, sfxSliderY + 5, 10, DARKGRAY);

    Rectangle sfxSliderRect = { (float)sliderX, (float)sfxSliderY - 10, (float)sliderW, 30 };
    if (!SoundManager::GetInstance().IsMuted() && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), sfxSliderRect)) {
        float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
        percent = std::max(0.0f, std::min(1.0f, percent));
        pauseSfxVolume = (int)(percent * 100);
        // Cập nhật tất cả SFX trong SoundManager
        SoundManager::GetInstance().SetSoundVol("COIN", pauseSfxVolume / 100.0f);
        SoundManager::GetInstance().SetSoundVol("COIN_COLLECTION", pauseSfxVolume / 100.0f);
        SoundManager::GetInstance().SetSoundVol("POWER_UP_APPEARS", pauseSfxVolume / 100.0f);
        SoundManager::GetInstance().SetSoundVol("ENEMY_DEATH", pauseSfxVolume / 100.0f);
        SoundManager::GetInstance().SetSoundVol("MARIO_JUMP", pauseSfxVolume / 100.0f);
        SoundManager::GetInstance().SetSoundVol("BUTTON_CLICK", pauseSfxVolume / 100.0f);
    }
    DrawText(TextFormat("%d", pauseSfxVolume), sliderX + sliderW + 20, sfxSliderY - 10, 20, BLACK);

    // --- MUTE ALL BUTTON ---
    Rectangle muteAllBtn = { (float)(winX + 150), (float)(winY + winH - 140), 100, 50 };
    DrawRectangleRec(muteAllBtn, DARKGRAY);
    DrawText(SoundManager::GetInstance().IsMuted() ? "Unmute All" : "Mute All", muteAllBtn.x + 10, muteAllBtn.y + 15, 24, WHITE);

    // --- Nút Resume ---
    Rectangle resumeBtn = { (float)(winX + 50), (float)(winY + winH - 80), 100, 50 };
    DrawRectangleRec(resumeBtn, GREEN);
    DrawText("Play", resumeBtn.x + 25, resumeBtn.y + 15, 24, WHITE);

    // --- Nút Home ---
    Rectangle homeBtn = { (float)(winX + 250), (float)(winY + winH - 80), 100, 50 };
    DrawRectangleRec(homeBtn, RED);
    DrawText("Home", homeBtn.x + 20, homeBtn.y + 15, 24, WHITE);

    // Xử lý click nút
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, muteAllBtn)) {
            bool isMuted = !SoundManager::GetInstance().IsMuted();
            SoundManager::GetInstance().SetMuted(isMuted);
            if (isMuted) {
                pauseMusicVolume = 0;
                pauseSfxVolume = 0;
                // Cập nhật tất cả music và SFX về 0
                SoundManager::GetInstance().SetMusicVol("MENU", 0.0f);
                SoundManager::GetInstance().SetMusicVol("GAMEWORLD_1", 0.0f);
                SoundManager::GetInstance().SetMusicVol("GAMEWORLD_2", 0.0f);
                SoundManager::GetInstance().SetMusicVol("GAMEWORLD_3", 0.0f);
                SoundManager::GetInstance().SetMusicVol("GAMEWORLD_4", 0.0f);
                SoundManager::GetInstance().SetSoundVol("COIN", 0.0f);
                SoundManager::GetInstance().SetSoundVol("COIN_COLLECTION", 0.0f);
                SoundManager::GetInstance().SetSoundVol("POWER_UP_APPEARS", 0.0f);
                SoundManager::GetInstance().SetSoundVol("ENEMY_DEATH", 0.0f);
                SoundManager::GetInstance().SetSoundVol("MARIO_JUMP", 0.0f);
                SoundManager::GetInstance().SetSoundVol("BUTTON_CLICK", 0.0f);
            } else {
                // Khôi phục giá trị từ SoundManager (được thiết lập bởi SettingsScreen)
                pauseMusicVolume = (int)(SoundManager::GetInstance().GetMusicVol("MENU") * 100.0f);
                pauseSfxVolume = (int)(SoundManager::GetInstance().GetSoundVol("COIN_COLLECTION") * 100.0f);
            }
        }
        if (CheckCollisionPointRec(mouse, resumeBtn)) {
            isPaused = false;
            showPauseMenu = false;
        }
        if (CheckCollisionPointRec(mouse, homeBtn)) {
            requestGoHome = true;
        }
    }
}

    if (BackMenu.IsPressed()) {
        screenController->ChangeScreen(new MenuScreen(screenController));
        return;
    }
    switch ( transitionState ) {
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
    }
    if (transitionState != TransitionState::NONE) {
        return;
    }

    switch ( gameWorld->GetGameState()) {

        case GameState::GAME_PLAYING:
            gameWorld->UpdateWorld();
            break;
        case GameState::GAME_COMPLETED:
            if (IsKeyPressed(KEY_ENTER)) {
                NextLevel();
                BeginTransition(TransitionState::NEXT_LEVEL);
            }
            break;
        case GameState::GAME_RESET:
            if (IsKeyPressed(KEY_ENTER)) {
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
            gameOverMusicPlayed = false; // Reset flag để lần sau có thể phát lại
            ResetGame();
            BeginTransition(TransitionState::GAME_OVER);
        }
            break;
        default:
        break;
    }
}


void GameScreen::BeginTransition(TransitionState transitionState) {
    this->transitionState = transitionState;
    transitionTimeAcum = 0.0f; // Reset the transition time accumulator
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
            SoundManager::GetInstance().PlayMusic("GAMEWORLD_" + std::to_string(level));
            break;
    }
}

void GameScreen::Draw() {
    gameWorld->DrawWorld();
    gameHUD->Draw();
    if (gameWorld->IsCompleted()) {
        if (transitionState == TransitionState::NONE) {
            DrawEnd();
        } 
    }
    // pause menu
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

        Rectangle musicSliderRect = { (float)sliderX, (float)sliderY - 10, (float)sliderW, 30 };
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), musicSliderRect)) {
            float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
            percent = std::max(0.0f, std::min(1.0f, percent));
            pauseMusicVolume = (int)(percent * 100);
            SoundManager::GetInstance().SetMusicVol("GAMEWORLD_" + std::to_string(level), pauseMusicVolume / 100.0f);
        }
        DrawText(TextFormat("%d", pauseMusicVolume), sliderX + sliderW + 20, sliderY - 10, 20, BLACK);

        // --- SFX SLIDER ---
        int sfxSliderY = sliderY + 70;
        DrawText("SFX Volume", sliderX, sfxSliderY - 30, 20, DARKGRAY);
        DrawRectangle(sliderX, sfxSliderY, sliderW, 10, LIGHTGRAY);
        DrawRectangle(sliderX, sfxSliderY, (int)(pauseSfxVolume * (sliderW/100.0f)), 10, ORANGE);
        int sfxKnobX = sliderX + (int)(pauseSfxVolume * (sliderW/100.0f));
        DrawCircle(sfxKnobX, sfxSliderY + 5, 10, DARKGRAY);

        Rectangle sfxSliderRect = { (float)sliderX, (float)sfxSliderY - 10, (float)sliderW, 30 };
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), sfxSliderRect)) {
            float percent = (GetMousePosition().x - sliderX) / (float)sliderW;
            percent = std::max(0.0f, std::min(1.0f, percent));
            pauseSfxVolume = (int)(percent * 100);
            // Nếu muốn áp dụng cho tất cả SFX, hãy lặp qua các sound, ở đây ví dụ với "COIN"
            SoundManager::GetInstance().SetSoundVol("COIN", pauseSfxVolume / 100.0f);
        }
        DrawText(TextFormat("%d", pauseSfxVolume), sliderX + sliderW + 20, sfxSliderY - 10, 20, BLACK);

        // --- Nút Resume ---
        Rectangle resumeBtn = { (float)(winX + 50), (float)(winY + winH - 80), 100, 50 };
        DrawRectangleRec(resumeBtn, GREEN);
        DrawText("Play", resumeBtn.x + 25, resumeBtn.y + 15, 24, WHITE);

        // --- Nút Home ---
        Rectangle homeBtn = { (float)(winX + 250), (float)(winY + winH - 80), 100, 50 };
        DrawRectangleRec(homeBtn, RED);
        DrawText("Home", homeBtn.x + 20, homeBtn.y + 15, 24, WHITE);

        // Xử lý click nút
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();
            if (CheckCollisionPointRec(mouse, resumeBtn)) {
                isPaused = false;
                showPauseMenu = false;
            }
            if (CheckCollisionPointRec(mouse, homeBtn)) {
                requestGoHome = true;
            }
        }
    }
    // game
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
                ("Level " + std::to_string(level )).c_str(),//fgdgdfgdf
                20,7).x/2,// thisssbad
                (float)GetScreenHeight() / 2 -100}, //dvsvsv
                20, 7, WHITE);
            DrawTextureNPatch(*SmallMario,//fbdb
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
            gameWorld->player2->SetLives(currentLives2  -1); // Reset Player 2 lives
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
        // gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    } else {
        // gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
    }
}

void GameScreen::DrawEnd() {
    static const Texture2D *LevelEndCongratulation = &ResrcManager::GetInstance().getTexture("LEVEL_END_CONGRATULATIONS");
    static const Texture2D *LevelEndEnter = &ResrcManager::GetInstance().getTexture("LEVEL_END_ENTER");
    DrawRectangleRounded(Rectangle{(float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 350, 800, 700}, 0.2f, 180, Color{255, 245, 137, 220}); // Semi-transparent background
      DrawRectangleRoundedLinesEx(Rectangle{(float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 350, 800, 700}, 0.2f, 180, 10.0f, Color{234,136,65,255}); // Border around the rectangle

    // Display summary or transition effects
    DrawTextureNPatch((*LevelEndCongratulation),
                       NPatchInfo{Rectangle{0, 0, (float)(*LevelEndCongratulation).width,
                                           (float)(*LevelEndCongratulation).height}, 0, 0, 0, 0},
                       Rectangle{(float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 300, 800, 120}, Vector2{0, 0}, 0.0f, WHITE);

    // Display the level completion message
      std::string summarry = "Level " + std::to_string(level) + " Complete!";
      DrawTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"), summarry.c_str(),
                       Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"),(summarry).c_str(),40,2).x / 2, (float)GetScreenHeight() / 2 - 150}, 40, 2, WHITE);

    // Display the player's score
                        std::string score = "Score: " + std::to_string(gameWorld->player1->GetScore() );
    DrawTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"), score.c_str(),
            Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"),(score).c_str(),60,2).x / 2, (float)GetScreenHeight() / 2 }, 60, 2, WHITE);
      
      //Display level end enter message
      DrawTextureNPatch(*LevelEndEnter,
                       NPatchInfo{Rectangle{0, 0, (float)(*LevelEndEnter).width,
                                           (float)(*LevelEndEnter).height}, 0, 0, 0, 0},
                       Rectangle{(float)GetScreenWidth() / 2 -700, (float)GetScreenHeight() / 2+100 , 1400, 400}, Vector2{0, 0}, 0.0f, WHITE);
      DrawTextureNPatch(ResrcManager::GetInstance().getTexture("HUD_COINS"),
                       NPatchInfo{Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("HUD_COINS").width,
                                           (float)ResrcManager::GetInstance().getTexture("HUD_COINS").height}, 0, 0, 0, 0},
                       Rectangle{(float)GetScreenWidth() / 2 - 200, (float)GetScreenHeight() / 2 + 100, 70, 70}, Vector2{0, 0}, 0.0f, WHITE);
        std::string coins = "X " + std::to_string(gameWorld->player1->GetCoins());
        DrawTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"), coins.c_str(),
                         Vector2{(float)GetScreenWidth() / 2+50 - MeasureTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"),(coins).c_str(),70,2).x / 2, (float)GetScreenHeight() / 2 + 105}, 70, 2, WHITE);
}

void GameScreen::NextLevel() {
    // Lưu thông tin trước khi chuyển level
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
    
    level++;
    if (level > 4) {
        level = 1; // Reset to first level if exceeded
    }
    
    // Tạo GameWorld mới
    gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);
    
    // Khôi phục thông tin Mario
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