#include "../inc/Screen/GameScreen.h"
#include "../inc/Screen/MenuScreen.h"
#include "../inc/Screen/MapSelectScreen.h"
#include "SoundManager.h"
#include <raylib.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include "json.hpp"

GameScreen::GameScreen(ScreenController *screenController)
    : Screen(screenController),
      BackMenu(Vector2{50, 50}, Vector2{50, 50}),
      level(5),
      transitionState(TransitionState::NONE),
      transitionTime(1.0f),
      transitionTimeAcum(0.0f),
      gameHUD(nullptr),
      isMultiplayer(false),
      PlayButton(Vector2{(float)PauseScreenPosX + 50, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "PLAY", 20, 1),
      MenuButton(Vector2{(float)PauseScreenPosX + 200, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "MENU", 20, 1),
      SaveButton(Vector2{(float)PauseScreenPosX + 350, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "SAVE", 20, 1),
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

GameScreen::GameScreen(ScreenController *screenController, bool multiplayer,
                       CharacterType p1Type, CharacterType p2Type)
    : Screen(screenController),
      BackMenu(Vector2{50, 50}, Vector2{50, 50}),
      level(5),
      transitionState(TransitionState::NONE),
      transitionTime(1.0f),
      transitionTimeAcum(0.0f),
      gameHUD(nullptr),
      PlayButton(Vector2{(float)PauseScreenPosX + 50, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "PLAY", 20, 1),
      MenuButton(Vector2{(float)PauseScreenPosX + 200, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "MENU", 20, 1),
      SaveButton(Vector2{(float)PauseScreenPosX + 350, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "SAVE", 20, 1),
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

    if (multiplayer && gameWorld->player2)
    {
        gameWorld->player2->SetLives(3);
        gameWorld->player2->SetCoins(0);
        gameWorld->player2->SetScore(0);
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    }
    else
    {
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

GameScreen::GameScreen(ScreenController *screenController, bool multiplayer,
                       CharacterType p1Type, CharacterType p2Type, MapType map, DifficultyLevel difficulty)
    : Screen(screenController),
      BackMenu(Vector2{50, 50}, Vector2{50, 50}),
      level(1),
      transitionState(TransitionState::NONE),
      transitionTime(1.0f),
      PlayButton(Vector2{(float)PauseScreenPosX + 50, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "PLAY", 20, 1),
      MenuButton(Vector2{(float)PauseScreenPosX + 200, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "MENU", 20, 1),
      SaveButton(Vector2{(float)PauseScreenPosX + 350, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "SAVE", 20, 1),
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
    switch (map)
    {
    case MapType::MAP_TUTORIAL:
        level = 0;
        break;
    case MapType::MAP_1:
        level = 1;
        break;
    case MapType::MAP_2:
        level = 2;
        break;
    case MapType::MAP_3:
        level = 3;
        break;
    case MapType::MAP_BOSS:
        level = 4;
        break;
    default:
        level = 1;
        break;
    }

    // Calculate enemy speed multiplier based on both map level and difficulty
    float baseSpeedMultiplier = 1.0f;
    float levelMultiplier = 1.0f;

    switch (difficulty)
    {
    case DifficultyLevel::EASY:
        baseSpeedMultiplier = 0.6f;
        break;
    case DifficultyLevel::MEDIUM:
        baseSpeedMultiplier = 1.0f;
        break;
    case DifficultyLevel::HARD:
        baseSpeedMultiplier = 1.8f;
        break;
    default:
        baseSpeedMultiplier = 1.0f;
        break;
    }
    switch (level)
    {
    case 1:
        levelMultiplier = 1.0f;
        break;
    case 2:
        levelMultiplier = 2.0f;
        break;
    case 3:
        levelMultiplier = 4.0f;
        break;
    default:
        levelMultiplier = 1.0f;
        break;
    }
    float enemySpeedMultiplier = baseSpeedMultiplier * levelMultiplier;

    gameWorld = std::make_unique<GameWorld>(level, this, multiplayer, p1Type, p2Type, enemySpeedMultiplier);

    // Initialize Player 1
    int initialLives = 3;
    gameWorld->player1->SetLives(initialLives);
    gameWorld->player1->SetCoins(0);
    gameWorld->player1->SetScore(0);

    // Initialize Player 2 if multiplayer
    if (multiplayer && gameWorld->player2)
    {
        gameWorld->player2->SetLives(initialLives);
        gameWorld->player2->SetCoins(0);
        gameWorld->player2->SetScore(0);
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    }
    else
    {
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

// New constructor to load from a saved map JSON path (resources/save/*.json)
GameScreen::GameScreen(ScreenController *screenController, const std::string &savedMapPath)
    : Screen(screenController),
      BackMenu(Vector2{50, 50}, Vector2{50, 50}),
      level(1),
      transitionState(TransitionState::NONE),
      transitionTime(1.0f),
      PlayButton(Vector2{(float)PauseScreenPosX + 50, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "PLAY", 20, 1),
      MenuButton(Vector2{(float)PauseScreenPosX + 200, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "MENU", 20, 1),
      SaveButton(Vector2{(float)PauseScreenPosX + 350, (float)PauseScreenPosY + 300}, Vector2{100, 50}, "SAVE", 20, 1),
      transitionTimeAcum(0.0f),
      gameHUD(nullptr),
      isMultiplayer(false),
      player1Type(CharacterType::MARIO),
      player2Type(CharacterType::LUIGI),
      loadedSavePath(savedMapPath)
{
    nlohmann::json j;
    bool parsed = false;
    if (std::filesystem::exists(savedMapPath))
    {
        std::ifstream f(savedMapPath);
        try
        {
            f >> j;
            parsed = true;
        }
        catch (...)
        {
            std::cout << "[SaveLoad] Failed to parse json: " << savedMapPath << std::endl;
        }
    }
    else
    {
        std::cout << "[SaveLoad] Saved map path not found: " << savedMapPath << std::endl;
    }

    // Detect saved level & multiplayer before constructing GameWorld so background matches
    if (parsed)
    {
        if (j.contains("level"))
        {
            int savedLevel = j.value("level", 1);
            if (savedLevel >= 0 && savedLevel <= 9)
            {
                level = savedLevel; // overwrite default so constructor picks correct background
            }
        }
        if (j.contains("player2"))
        {
            isMultiplayer = true;
        }
    }

    // If json includes characterType, override defaults before constructing world
    if (parsed && j.contains("player1") && j["player1"].contains("characterType"))
    {
        player1Type = static_cast<CharacterType>(j["player1"].value("characterType", (int)CharacterType::MARIO));
    }
    if (parsed && isMultiplayer && j.contains("player2") && j["player2"].contains("characterType"))
    {
        player2Type = static_cast<CharacterType>(j["player2"].value("characterType", (int)CharacterType::LUIGI));
    }
    // Construct world with correct multiplayer + character choices
    gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);

    if (parsed)
    {
        if (j.contains("layers"))
        {
            std::filesystem::path tempPath = std::filesystem::temp_directory_path() / "_temp_saved_map_load.json";
            try
            {
                std::ofstream tmp(tempPath);
                tmp << j.dump();
                tmp.close();
                gameWorld->map.Clear();
                gameWorld->map.LoadFromJsonFile(tempPath.string());
            }
            catch (...)
            {
                std::cout << "[SaveLoad] Could not rebuild map from saved data" << std::endl;
            }
        }
        if (j.contains("player1"))
        {
            auto &p = j["player1"];
            float px = p.value("x", 100.0f);
            float py = p.value("y", 100.0f);
            gameWorld->player1->SetPos(Vector2{px, py});
            gameWorld->player1->SetLives(p.value("lives", 3));
            gameWorld->player1->SetCoins(p.value("coins", 0));
            gameWorld->player1->SetScore(p.value("score", 0));
            gameWorld->player1->SetMarioState(static_cast<ObjectState>(p.value("state", (int)SMALL)));
            if (p.contains("additionalState"))
                gameWorld->player1->SetAdditionalState(static_cast<ObjectState>(p.value("additionalState", 0)));
        }
        if (isMultiplayer && gameWorld->player2 && j.contains("player2"))
        {
            auto &p = j["player2"];
            float px = p.value("x", 120.0f);
            float py = p.value("y", 100.0f);
            gameWorld->player2->SetPos(Vector2{px, py});
            gameWorld->player2->SetLives(p.value("lives", 3));
            gameWorld->player2->SetCoins(p.value("coins", 0));
            gameWorld->player2->SetScore(p.value("score", 0));
            gameWorld->player2->SetMarioState(static_cast<ObjectState>(p.value("state", (int)SMALL)));
            if (p.contains("additionalState"))
                gameWorld->player2->SetAdditionalState(static_cast<ObjectState>(p.value("additionalState", 0)));
        }
        std::cout << "[SaveLoad] Loaded saved map & player data: " << savedMapPath << std::endl;
    }
    else
    {
        // Defaults if not parsed
        gameWorld->player1->SetLives(3);
        gameWorld->player1->SetCoins(0);
        gameWorld->player1->SetScore(0);
        if (isMultiplayer && gameWorld->player2)
        {
            gameWorld->player2->SetLives(3);
            gameWorld->player2->SetCoins(0);
            gameWorld->player2->SetScore(0);
        }
    }

    // HUD depends on multiplayer
    if (isMultiplayer && gameWorld->player2)
    {
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    }
    else
    {
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
    }

    BackMenu.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
    pauseMusicVolume = (int)(SoundManager::GetInstance().GetMusicVol("MENU") * 100.0f);
    pauseSfxVolume = (int)(SoundManager::GetInstance().GetSoundVol("COIN_COLLECTION") * 100.0f);
    SoundManager::GetInstance().StopAllSounds();
    string musicKey = "GAMEWORLD_" + std::to_string(level);
    SoundManager::GetInstance().SetMusicVol(musicKey, pauseMusicVolume / 100.0f);
    SoundManager::GetInstance().PlayMusic(musicKey);
}

void GameScreen::Update()
{
    // Cập nhật nút Back
    BackMenu.Update();

    if (BackMenu.IsPressed())
    {
        if (!SoundManager::GetInstance().IsMuted())
        {
            SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        }
        screenController->ChangeScreen(new MenuScreen(screenController));
        return;
    }

    if (requestGoHome)
    {
        if (!SoundManager::GetInstance().IsMuted())
        {
            SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        }
        screenController->ChangeScreen(new MenuScreen(screenController));
        return;
    }

    // Toggle pause/resume bằng phím P
    if (IsKeyPressed(KEY_P))
    {
        isPaused = !isPaused;
        showPauseMenu = isPaused;
    }

    if (showPauseMenu && !showSaveDialog)
    {
        // Cửa sổ pause
        int winW = 400, winH = 350;
        int winX = GetScreenWidth() / 2 - winW / 2;
        int winY = GetScreenHeight() / 2 - winH / 2;
        int sliderW = 200;
        int sliderX = winX + 100;
        int sliderY = winY + 80;

        // Cập nhật các nút trong pause menu
        PlayButton.Update();
        MenuButton.Update();
        SaveButton.Update();

        // --- MUSIC SLIDER ---
        Rectangle musicSliderRect = {(float)sliderX, (float)sliderY - 10, (float)sliderW, 30};
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), musicSliderRect))
        {
            if (SoundManager::GetInstance().IsMuted())
            {
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
        Rectangle sfxSliderRect = {(float)sliderX, (float)sfxSliderY - 10, (float)sliderW, 30};
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), sfxSliderRect))
        {
            if (SoundManager::GetInstance().IsMuted())
            {
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

        // --- Nút Play, Menu, Save ---
        if (PlayButton.IsPressed())
        {
            if (!SoundManager::GetInstance().IsMuted())
            {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            isPaused = false;
            showPauseMenu = false;
        }
        if (MenuButton.IsPressed())
        {
            if (!SoundManager::GetInstance().IsMuted())
            {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            requestGoHome = true;
        }
        if (SaveButton.IsPressed())
        {
            showSaveDialog = true;
            memset(saveFileName, 0, sizeof(saveFileName));
            saveNameLength = 0;
            if (!loadedSavePath.empty())
            {
                try
                {
                    std::filesystem::path p(loadedSavePath);
                    std::string stem = p.stem().string();
                    if (!stem.empty() && stem.size() < sizeof(saveFileName))
                    {
                        std::strncpy(saveFileName, stem.c_str(), sizeof(saveFileName) - 1);
                        saveNameLength = (int)stem.size();
                    }
                }
                catch (...)
                { /* ignore */
                }
            }
        }

        // Debug log
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            std::cout << "[GameScreen Debug] Mouse pos: (" << mousePos.x << ", " << mousePos.y
                      << "), sfxSliderRect: (" << sfxSliderRect.x << ", " << sfxSliderRect.y
                      << ", " << sfxSliderRect.width << ", " << sfxSliderRect.height << ")"
                      << ", collision: " << CheckCollisionPointRec(GetMousePosition(), sfxSliderRect) << std::endl;
        }
        if(!showSaveDialog) return;
    }

    if (showSaveDialog)
    {
        // Handle text input
        int key = GetCharPressed();
        while (key > 0)
        {
            if (key >= 32 && key <= 125 && saveNameLength < 63)
            {
                saveFileName[saveNameLength] = (char)key;
                saveNameLength++;
            }
            key = GetCharPressed();
        }

        // Handle backspace
        if (IsKeyPressed(KEY_BACKSPACE) && saveNameLength > 0)
        {
            saveNameLength--;
            saveFileName[saveNameLength] = '\0';
        }

        // Handle Enter - save with the entered name
        if (IsKeyPressed(KEY_ENTER) && saveNameLength > 0)
        {
            std::string typedName(saveFileName);
            if (!loadedSavePath.empty())
            {
                std::filesystem::path p(loadedSavePath);
                std::string original = p.stem().string();
                if (_stricmp(original.c_str(), typedName.c_str()) == 0)
                {
                    SaveMapInSettings(original);
                }
                else
                {
                    SaveMapInSettings(typedName);
                }
            }
            else
            {
                SaveMapInSettings(typedName);
            }
            showSaveDialog = false;
        }

        // Handle Escape - cancel save dialog
        if (IsKeyPressed(KEY_ESCAPE))
        {
            showSaveDialog = false;
        }

        return;
    }

    if (showWinScreen)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            if (!SoundManager::GetInstance().IsMuted())
            {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            showWinScreen = false;
            screenController->ChangeScreen(new MenuScreen(screenController));
        }
        return;
    }

    switch (transitionState)
    {
    case TransitionState::NEXT_LEVEL:
        transitionTimeAcum += GameClock::GetInstance().FIXED_TIME_STEP;
        if (transitionTimeAcum >= transitionTime)
        {
            BeginTransition(TransitionState::NONE);
        }
        break;
    case TransitionState::GAME_OVER:
        transitionTimeAcum += GameClock::GetInstance().FIXED_TIME_STEP;
        if (transitionTimeAcum >= transitionTime)
        {
            BeginTransition(TransitionState::NONE);
        }
        break;
    case TransitionState::GAME_RESET:
        transitionTimeAcum += GameClock::GetInstance().FIXED_TIME_STEP;
        if (transitionTimeAcum >= transitionTime)
        {
            BeginTransition(TransitionState::NONE);
        }
        break;
    }

    if (transitionState != TransitionState::NONE)
    {
        return;
    }

    switch (gameWorld->GetGameState())
    {
    case GameState::GAME_PLAYING:
        if (isPaused)
            break;
        gameWorld->UpdateWorld();
        break;
    case GameState::GAME_COMPLETED:
        if (IsKeyPressed(KEY_ENTER))
        {
            if (!SoundManager::GetInstance().IsMuted())
            {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            NextLevel();
            BeginTransition(TransitionState::NEXT_LEVEL);
        }
        break;
    case GameState::GAME_RESET:
        if (IsKeyPressed(KEY_ENTER))
        {
            if (!SoundManager::GetInstance().IsMuted())
            {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
            }
            ResetGame();
            BeginTransition(TransitionState::GAME_RESET);
        }
        break;
    case GameState::GAME_OVER:
        static bool gameOverMusicPlayed = false;
        if (!gameOverMusicPlayed)
        {
            SoundManager::GetInstance().StopAllSounds();
            SoundManager::GetInstance().PlayMusic("GAME_OVER");
            gameOverMusicPlayed = true;
        }
        if (IsKeyPressed(KEY_ENTER))
        {
            if (!SoundManager::GetInstance().IsMuted())
            {
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

void GameScreen::Draw()
{
    if (showWinScreen)
    {
        Texture2D &winTex = ResrcManager::GetInstance().getTexture("WIN");
        DrawTexturePro(
            winTex,
            {0, 0, (float)winTex.width, (float)winTex.height},
            {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            {0, 0},
            0.0f,
            WHITE);
        DrawText("Press ENTER to return to menu", GetScreenWidth() / 2 - 200, GetScreenHeight() - 100, 32, BLACK);
        return;
    }

    gameWorld->DrawWorld();
    gameHUD->Draw();
    if (gameWorld->IsCompleted())
    {
        if (transitionState == TransitionState::NONE)
        {
            DrawEnd();
        }
    }
    if (showPauseMenu)
    {
        // Overlay mờ
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));

        // Cửa sổ pause
        int winW = 400, winH = 350;
        int winX = GetScreenWidth() / 2 - winW / 2;
        int winY = GetScreenHeight() / 2 - winH / 2;
        DrawRectangleRounded({(float)winX, (float)winY, (float)winW, (float)winH}, 0.2f, 10, WHITE);
        DrawText("PAUSED", winX + 140, winY + 20, 32, BLACK);

        // --- MUSIC SLIDER ---
        int sliderW = 200;
        int sliderX = winX + 100;
        int sliderY = winY + 80;
        DrawText("Music Volume", sliderX, sliderY - 30, 20, DARKGRAY);
        DrawRectangle(sliderX, sliderY, sliderW, 10, LIGHTGRAY);
        DrawRectangle(sliderX, sliderY, (int)(pauseMusicVolume * (sliderW / 100.0f)), 10, BLUE);
        int knobX = sliderX + (int)(pauseMusicVolume * (sliderW / 100.0f));
        DrawCircle(knobX, sliderY + 5, 10, DARKBLUE);
        DrawText(TextFormat("%d%%", pauseMusicVolume), sliderX + sliderW + 20, sliderY - 10, 20, BLACK);

        // --- SFX SLIDER ---
        int sfxSliderY = sliderY + 70;
        DrawText("SFX Volume", sliderX, sfxSliderY - 30, 20, DARKGRAY);
        DrawRectangle(sliderX, sfxSliderY, sliderW, 10, LIGHTGRAY);
        DrawRectangle(sliderX, sfxSliderY, (int)(pauseSfxVolume * (sliderW / 100.0f)), 10, ORANGE);
        int sfxKnobX = sliderX + (int)(pauseSfxVolume * (sliderW / 100.0f));
        DrawCircle(sfxKnobX, sfxSliderY + 5, 10, DARKGRAY);
        DrawText(TextFormat("%d%%", pauseSfxVolume), sliderX + sliderW + 20, sfxSliderY - 10, 20, BLACK);

        // Vẽ các nút Play, Menu, Save
        PlayButton.Draw();
        MenuButton.Draw();
        SaveButton.Draw();
    }

    // Draw save dialog
    if (showSaveDialog)
    {
        // Draw overlay
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

        // Dialog box
        int dialogW = 400;
        int dialogH = 200;
        int dialogX = GetScreenWidth() / 2 - dialogW / 2;
        int dialogY = GetScreenHeight() / 2 - dialogH / 2;

        DrawRectangle(dialogX, dialogY, dialogW, dialogH, WHITE);
        DrawRectangleLines(dialogX, dialogY, dialogW, dialogH, BLACK);

        // Title
        const char *title = "Enter Save Name";
        int titleW = MeasureText(title, 24);
        DrawText(title, dialogX + dialogW / 2 - titleW / 2, dialogY + 20, 24, BLACK);

        // Input box
        int inputX = dialogX + 20;
        int inputY = dialogY + 70;
        int inputW = dialogW - 40;
        int inputH = 40;

        DrawRectangle(inputX, inputY, inputW, inputH, LIGHTGRAY);
        DrawRectangleLines(inputX, inputY, inputW, inputH, DARKGRAY);

        // Display current text
        DrawText(saveFileName, inputX + 5, inputY + 10, 20, BLACK);

        // Cursor blink
        if (((int)(GetTime() * 2)) % 2 == 0)
        {
            int textW = MeasureText(saveFileName, 20);
            DrawText("_", inputX + 5 + textW, inputY + 10, 20, BLACK);
        }

        // Instructions
        const char *instruction1 = "Press ENTER to save";
        const char *instruction2 = "Press ESC to cancel";
        int inst1W = MeasureText(instruction1, 16);
        int inst2W = MeasureText(instruction2, 16);
        DrawText(instruction1, dialogX + dialogW / 2 - inst1W / 2, dialogY + 130, 16, DARKGRAY);
        DrawText(instruction2, dialogX + dialogW / 2 - inst2W / 2, dialogY + 150, 16, DARKGRAY);
    }

    // Game over and reset screens
    Texture *GameOver = &ResrcManager::GetInstance().getTexture("GAME_OVER");
    Font *SuperMarioFont = &ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT");
    Texture *SmallMario = &ResrcManager::GetInstance().getTexture("SMALL_MARIO_0_RIGHT");
    if (gameWorld->GetGameState() == GameState::GAME_OVER && transitionState == TransitionState::NONE)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
        DrawTextureNPatch(*GameOver, NPatchInfo{Rectangle{0, 0, (float)(*GameOver).width, (float)(*GameOver).height}, 0, 0, 0, 0}, Rectangle{(float)GetScreenWidth() / 2 - 320, (float)GetScreenHeight() / 2 - 150, 640, 128}, Vector2{0, 0}, 0.0f, WHITE);
        DrawTextEx(*SuperMarioFont, "Press ENTER to restart", Vector2{(float)GetScreenWidth() / 2 - 200, (float)GetScreenHeight() / 2 + 100}, 20, 7, WHITE);
    }
    if (gameWorld->GetGameState() == GameState::GAME_RESET && transitionState == TransitionState::NONE)
    {
        DrawTextEx(*SuperMarioFont,
                   "Press ENTER to restart",
                   Vector2{(float)GetScreenWidth() / 2 - 200,
                           (float)GetScreenHeight() / 2 + 100},
                   20, 7, WHITE);
    }

    switch (transitionState)
    {
    case TransitionState::NEXT_LEVEL:
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
        DrawTextEx(*SuperMarioFont, ("Level " + std::to_string(level)).c_str(),
                   Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(
                                                             *SuperMarioFont,
                                                             ("Level " + std::to_string(level)).c_str(), 20, 7)
                                                                 .x /
                                                             2,
                           (float)GetScreenHeight() / 2 - 100},
                   20, 7, WHITE);
        DrawTextureNPatch(*SmallMario,
                          NPatchInfo{Rectangle{0, 0, (float)(*SmallMario).width, 24}, 0, 0, 0, 0},
                          Rectangle{(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 16, 43, 32}, Vector2{0, 0}, 0.0f, WHITE);

        Vector2 size = MeasureTextEx(*SuperMarioFont,
                                     ("X " + std::to_string(level + 1)).c_str(), 20, 7);
        DrawTextEx(*SuperMarioFont, ("X " + std::to_string(gameWorld->player1->GetLives())).c_str(),
                   Vector2{(float)GetScreenWidth() / 2 - size.x / 2,
                           (float)GetScreenHeight() / 2 - size.y / 2},
                   20, 7, WHITE);
        break;
    }
    case TransitionState::GAME_RESET:
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
        DrawTextEx(*SuperMarioFont, ("Level " + std::to_string(level)).c_str(),
                   Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(
                                                             *SuperMarioFont,
                                                             ("Level " + std::to_string(level)).c_str(),
                                                             20, 7)
                                                                 .x /
                                                             2,
                           (float)GetScreenHeight() / 2 - 100},
                   20, 7, WHITE);
        DrawTextureNPatch(*SmallMario,
                          NPatchInfo{Rectangle{0, 0, (float)(*SmallMario).width,
                                               24},
                                     0, 0, 0, 0},
                          Rectangle{(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 16, 43, 32}, Vector2{0, 0}, 0.0f, WHITE);
        Vector2 size = MeasureTextEx(*SuperMarioFont,
                                     ("X " + std::to_string(level + 1)).c_str(), 20, 7);
        DrawTextEx(*SuperMarioFont, ("X " + std::to_string(gameWorld->player1->GetLives())).c_str(),
                   Vector2{(float)GetScreenWidth() / 2 - size.x / 2,
                           (float)GetScreenHeight() / 2 - size.y / 2},
                   20, 7, WHITE);
        break;
    }
    case TransitionState::GAME_OVER:
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
        DrawTextEx(*SuperMarioFont, ("Level " + std::to_string(level)).c_str(),
                   Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(
                                                             *SuperMarioFont,
                                                             ("Level " + std::to_string(level)).c_str(),
                                                             20, 7)
                                                                 .x /
                                                             2,
                           (float)GetScreenHeight() / 2 - 100},
                   20, 7, WHITE);
        DrawTextureNPatch(*SmallMario,
                          NPatchInfo{Rectangle{0, 0, (float)(*SmallMario).width,
                                               24},
                                     0, 0, 0, 0},
                          Rectangle{(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 16, 43, 32}, Vector2{0, 0}, 0.0f, WHITE);
        Vector2 size = MeasureTextEx(*SuperMarioFont,
                                     ("X " + std::to_string(level + 1)).c_str(), 20, 7);
        DrawTextEx(*SuperMarioFont, ("X " + std::to_string(gameWorld->player1->GetLives())).c_str(),
                   Vector2{(float)GetScreenWidth() / 2 - size.x / 2,
                           (float)GetScreenHeight() / 2 - size.y / 2},
                   20, 7, WHITE);
        break;
    }
    }
    BackMenu.Draw();
}

void GameScreen::ResetGame()
{
    // Lưu thông tin trước khi xóa gameWorld
    int currentLives1 = gameWorld->player1->GetLives();
    int currentLives2 = gameWorld->player2 ? gameWorld->player2->GetLives() : 0;
    if (currentLives1 > 1 || (isMultiplayer && currentLives2 > 1))
    {
        gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);
        gameWorld->player1->SetLives(currentLives1 - 1);
        if (isMultiplayer && gameWorld->player2)
        {
            gameWorld->player2->SetLives(currentLives2 - 1);
        }
    }
    else
    {
        level = 1;
        gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);
        gameWorld->player1->SetLives(3);
        gameWorld->player1->SetCoins(0);
        gameWorld->player1->SetScore(0);
        if (isMultiplayer && gameWorld->player2)
        {
            gameWorld->player2->SetLives(3);
            gameWorld->player2->SetCoins(0);
            gameWorld->player2->SetScore(0);
        }
    }
    if (isMultiplayer)
    {
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    }
    else
    {
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
    }
}

void GameScreen::DrawEnd()
{
    static const Texture2D *LevelEndCongratulation = &ResrcManager::GetInstance().getTexture("LEVEL_END_CONGRATULATIONS");
    static const Texture2D *LevelEndEnter = &ResrcManager::GetInstance().getTexture("LEVEL_END_ENTER");
    DrawRectangleRounded(Rectangle{(float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 350, 800, 700}, 0.2f, 180, Color{255, 245, 137, 220});
    DrawRectangleRoundedLinesEx(Rectangle{(float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 350, 800, 700}, 0.2f, 180, 10.0f, Color{234, 136, 65, 255});

    DrawTextureNPatch((*LevelEndCongratulation),
                      NPatchInfo{Rectangle{0, 0, (float)(*LevelEndCongratulation).width,
                                           (float)(*LevelEndCongratulation).height},
                                 0, 0, 0, 0},
                      Rectangle{(float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 300, 800, 120}, Vector2{0, 0}, 0.0f, WHITE);
    std::string summarry = "Level " + std::to_string(level) + " Complete!";
    DrawTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"), summarry.c_str(),
               Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"), (summarry).c_str(), 40, 2).x / 2, (float)GetScreenHeight() / 2 - 150}, 40, 2, WHITE);

    std::string score = "Score: " + std::to_string(gameWorld->player1->GetScore());
    DrawTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"), score.c_str(),
               Vector2{(float)GetScreenWidth() / 2 - MeasureTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"), (score).c_str(), 60, 2).x / 2, (float)GetScreenHeight() / 2}, 60, 2, WHITE);

    DrawTextureNPatch(*LevelEndEnter,
                      NPatchInfo{Rectangle{0, 0, (float)(*LevelEndEnter).width,
                                           (float)(*LevelEndEnter).height},
                                 0, 0, 0, 0},
                      Rectangle{(float)GetScreenWidth() / 2 - 700, (float)GetScreenHeight() / 2 + 100, 1400, 400}, Vector2{0, 0}, 0.0f, WHITE);
    DrawTextureNPatch(ResrcManager::GetInstance().getTexture("HUD_COINS"),
                      NPatchInfo{Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("HUD_COINS").width,
                                           (float)ResrcManager::GetInstance().getTexture("HUD_COINS").height},
                                 0, 0, 0, 0},
                      Rectangle{(float)GetScreenWidth() / 2 - 200, (float)GetScreenHeight() / 2 + 100, 70, 70}, Vector2{0, 0}, 0.0f, WHITE);
    std::string coins = "X " + std::to_string(gameWorld->player1->GetCoins());
    DrawTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"), coins.c_str(),
               Vector2{(float)GetScreenWidth() / 2 + 50 - MeasureTextEx(ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT"), (coins).c_str(), 70, 2).x / 2, (float)GetScreenHeight() / 2 + 105}, 70, 2, WHITE);
}

void GameScreen::NextLevel()
{
    int currentLives1 = gameWorld->player1->GetLives();
    int currentCoins1 = gameWorld->player1->GetCoins();
    int currentScore1 = gameWorld->player1->GetScore();
    ObjectState currentPlayerState1 = gameWorld->player1->GetMarioState();

    int currentLives2 = isMultiplayer && gameWorld->player2 ? gameWorld->player2->GetLives() : 0;
    int currentCoins2 = isMultiplayer && gameWorld->player2 ? gameWorld->player2->GetCoins() : 0;
    int currentScore2 = isMultiplayer && gameWorld->player2 ? gameWorld->player2->GetScore() : 0;
    ObjectState currentPlayerState2 = SMALL;
    if (isMultiplayer && gameWorld->player2)
    {
        currentPlayerState2 = gameWorld->player2->GetMarioState();
    }
    MapType nextMap = selectedMap;
    DifficultyLevel nextDifficulty = selectedDifficulty;

    if (selectedMap == MapType::MAP_TUTORIAL)
    {
        nextMap = MapType::MAP_1;
        nextDifficulty = DifficultyLevel::EASY;
    }
    else if (selectedMap == MapType::MAP_1 && selectedDifficulty == DifficultyLevel::EASY)
    {
        nextMap = MapType::MAP_2;
        nextDifficulty = DifficultyLevel::EASY;
    }
    else if (selectedMap == MapType::MAP_2 && selectedDifficulty == DifficultyLevel::EASY)
    {
        nextMap = MapType::MAP_3;
        nextDifficulty = DifficultyLevel::EASY;
    }
    else if (selectedMap == MapType::MAP_3 && selectedDifficulty == DifficultyLevel::EASY)
    {
        nextMap = MapType::MAP_1;
        nextDifficulty = DifficultyLevel::MEDIUM;
    }
    else if (selectedMap == MapType::MAP_1 && selectedDifficulty == DifficultyLevel::MEDIUM)
    {
        nextMap = MapType::MAP_2;
        nextDifficulty = DifficultyLevel::MEDIUM;
    }
    else if (selectedMap == MapType::MAP_2 && selectedDifficulty == DifficultyLevel::MEDIUM)
    {
        nextMap = MapType::MAP_3;
        nextDifficulty = DifficultyLevel::MEDIUM;
    }
    else if (selectedMap == MapType::MAP_3 && selectedDifficulty == DifficultyLevel::MEDIUM)
    {
        nextMap = MapType::MAP_1;
        nextDifficulty = DifficultyLevel::HARD;
    }
    else if (selectedMap == MapType::MAP_1 && selectedDifficulty == DifficultyLevel::HARD)
    {
        nextMap = MapType::MAP_2;
        nextDifficulty = DifficultyLevel::HARD;
    }
    else if (selectedMap == MapType::MAP_2 && selectedDifficulty == DifficultyLevel::HARD)
    {
        nextMap = MapType::MAP_3;
        nextDifficulty = DifficultyLevel::HARD;
    }
    else if (selectedMap == MapType::MAP_3 && selectedDifficulty == DifficultyLevel::HARD)
    {
        nextMap = MapType::MAP_BOSS;
    }
    else if (selectedMap == MapType::MAP_BOSS)
    {
        // Hiển thị màn hình Course Cleared với texture WIN và nút về menu
        showWinScreen = true;
        return;
    }

    int nextLevel = 1;
    switch (nextMap)
    {
    case MapType::MAP_TUTORIAL:
        nextLevel = 0;
        break;
    case MapType::MAP_1:
        nextLevel = 1;
        break;
    case MapType::MAP_2:
        nextLevel = 2;
        break;
    case MapType::MAP_3:
        nextLevel = 3;
        break;
    case MapType::MAP_BOSS:
        nextLevel = 4;
        break;
    default:
        nextLevel = 1;
        break;
    }
    selectedMap = nextMap;
    selectedDifficulty = nextDifficulty;
    level = nextLevel;
    gameWorld = std::make_unique<GameWorld>(level, this, isMultiplayer, player1Type, player2Type);

    gameWorld->player1->SetLives(currentLives1);
    gameWorld->player1->SetCoins(currentCoins1);
    gameWorld->player1->SetScore(currentScore1);
    gameWorld->player1->SetMarioState(currentPlayerState1);
    if (isMultiplayer && gameWorld->player2)
    {
        gameWorld->player2->SetLives(currentLives2);
        gameWorld->player2->SetCoins(currentCoins2);
        gameWorld->player2->SetScore(currentScore2);
        gameWorld->player2->SetMarioState(currentPlayerState2);
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1, gameWorld->player2);
    }
    else
        gameHUD = std::make_unique<GameHUD>(gameWorld->player1);
}

void GameScreen::BeginTransition(TransitionState transitionState)
{
    this->transitionState = transitionState;
    transitionTimeAcum = 0.0f;
    switch (transitionState)
    {
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

std::string GameScreen::GetCurrentDateTime()
{
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void GameScreen::SaveMapInSettings(const std::string &fileName)
{
    std::string mapName = fileName.empty() ? "autosave" : fileName;
    std::filesystem::path dir = "resources/save";
    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    if (ec)
    {
        std::cout << "[Save] Cannot create directory resources/save : " << ec.message() << std::endl;
    }
    std::filesystem::path fullPath = dir / (mapName + ".json");

    try
    {
        nlohmann::json mapData;

        // Basic map info
        mapData["compressionlevel"] = -1;
        mapData["height"] = 30;
        mapData["infinite"] = false;
        mapData["orientation"] = "orthogonal";
        mapData["renderorder"] = "right-down";
        mapData["tiledversion"] = "1.11.2";
        mapData["tileheight"] = 32;
        mapData["tilewidth"] = 32;
        mapData["type"] = "map";
        mapData["version"] = "1.10";
        mapData["width"] = 150;
        mapData["nextlayerid"] = 2;
        mapData["nextobjectid"] = 1;
        mapData["level"] = level;
        // Create tileset reference
        mapData["tilesets"] = nlohmann::json::array();
        mapData["tilesets"].push_back({{"firstgid", 1},
                                       {"source", mapName + ".tsx"}});

        // Create layer data array (150 x 30 = 4500 tiles)
        std::vector<int> layerData(150 * 30, 0);

        // Add tiles to layer data
        for (auto &tile : gameWorld->GetMap()->getInteractiveTiles())
        {
            if (tile)
            {
                int x = (int)(tile->GetPos().x / 32);
                int y = (int)(tile->GetPos().y / 32);
                if (x >= 0 && x < 150 && y >= 0 && y < 30)
                {
                    layerData[y * 150 + x] = tile->GetKey();
                }
            }
        }

        // Add blocks to layer data
        for (auto &block : gameWorld->GetMap()->getBlocks())
        {
            if (block)
            {
                int x = (int)(block->GetPos().x / 32);
                int y = (int)(block->GetPos().y / 32);
                if (x >= 0 && x < 150 && y >= 0 && y < 30)
                {
                    int tileId = GetBlockTileId(block);
                    // Encode hit state for question blocks with separate IDs (avoid collisions with items)
                    // Base IDs: 116 empty, 117 coin, 118 fire
                    // Used versions: 170 (used empty), 171 (used coin), 172 (used fire)
                    if (block->isHit())
                    {
                        if (tileId == 116)
                            tileId = 170; // used empty
                        else if (tileId == 117)
                            tileId = 171; // used coin
                        else if (tileId == 118)
                            tileId = 172; // used fire
                    }
                    if (tileId > 0)
                    {
                        layerData[y * 150 + x] = tileId;
                    }
                }
            }
        }

        // Add enemies to layer data
        for (auto &enemy : gameWorld->GetMap()->GetEnemies())
        {
            if (enemy)
            {
                int x = (int)(enemy->GetPos().x / 32);
                int y = (int)((enemy->GetPos().y + 20) / 32); // Adjust for enemy positioning
                if (x >= 0 && x < 150 && y >= 0 && y < 30)
                {
                    int tileId = GetEnemyTileId(enemy);
                    if (tileId > 0)
                    {
                        layerData[y * 150 + x] = tileId;
                    }
                }
            }
        }

        // Add items to layer data
        for (auto &item : gameWorld->GetMap()->GetInteractiveItems())
        {
            if (item)
            {
                int x = (int)(item->GetPos().x / 32);
                int y = (int)(item->GetPos().y / 32);
                if (x >= 0 && x < 150 && y >= 0 && y < 30)
                {
                    int tileId = GetItemTileId(item.get());
                    if (tileId > 0)
                    {
                        layerData[y * 150 + x] = tileId;
                    }
                }
            }
        }

        // Create layer
        mapData["layers"] = nlohmann::json::array();
        mapData["layers"].push_back({{"data", layerData},
                                     {"height", 30},
                                     {"id", 1},
                                     {"name", "Tile Layer 1"},
                                     {"opacity", 1},
                                     {"type", "tilelayer"},
                                     {"visible", true},
                                     {"width", 150},
                                     {"x", 0},
                                     {"y", 0}});

        // Player data (player1 always, player2 if multiplayer)
        nlohmann::json p1;
        p1["x"] = gameWorld->player1->GetPos().x;
        p1["y"] = gameWorld->player1->GetPos().y;
        p1["lives"] = gameWorld->player1->GetLives();
        p1["coins"] = gameWorld->player1->GetCoins();
        p1["score"] = gameWorld->player1->GetScore();
        p1["state"] = (int)gameWorld->player1->GetMarioState();
        p1["additionalState"] = (int)gameWorld->player1->GetAdditionalState();
        p1["characterType"] = (int)player1Type; // persist chosen character
        mapData["player1"] = p1;

        if (isMultiplayer && gameWorld->player2)
        {
            nlohmann::json p2;
            p2["x"] = gameWorld->player2->GetPos().x;
            p2["y"] = gameWorld->player2->GetPos().y;
            p2["lives"] = gameWorld->player2->GetLives();
            p2["coins"] = gameWorld->player2->GetCoins();
            p2["score"] = gameWorld->player2->GetScore();
            p2["state"] = (int)gameWorld->player2->GetMarioState();
            p2["additionalState"] = (int)gameWorld->player2->GetAdditionalState();
            p2["characterType"] = (int)player2Type;
            mapData["player2"] = p2;
        }

        // Write to file
        std::ofstream file(fullPath, std::ios::out | std::ios::trunc);
        if (!file.is_open())
        {
            std::cout << "[Save] Failed to open file: " << std::filesystem::absolute(fullPath) << std::endl;
            return;
        }
        file << mapData.dump(4); // Pretty print
        file.close();
        std::cout << "Map successfully saved to: " << std::filesystem::absolute(fullPath) << std::endl;
        std::cout << "Current working dir: " << std::filesystem::current_path() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error saving map: " << e.what() << std::endl;
    }
}

int GameScreen::GetBlockTileId(Block *block)
{
    if (dynamic_cast<QuestionBlock *>(block))
    {
        if (dynamic_cast<QuestionBlock *>(block)->GetGiftType() == GIFT_COIN)
            return 117; // Coin block
        if (dynamic_cast<QuestionBlock *>(block)->GetGiftType() == GIFT_FIRE_FLOWER)
            return 118; // Fire flower block
        return 116;     // Default question block
    }
    else if (dynamic_cast<WoodBlock *>(block))
        return 121;
    else if (auto gb = dynamic_cast<GlassBlock *>(block))
    {
        // 112 = intact, 149 = transitioning_1, 150 = transitioning_2, 151 = to be removed (skip saving?)
        switch (gb->GetState())
        {
        case OBJECT_STATE_TRANSITIONING_1:
            return 149;
        case OBJECT_STATE_TRANSITIONING_2:
            return 150;
        case OBJECT_STATE_TO_BE_REMOVED:
            return 151; // will not recreate block
        default:
            return 112;
        }
    }
    else if (dynamic_cast<CloudBlock *>(block))
        return 105;
    else if (dynamic_cast<EyesClosedBlock *>(block))
        return 107; // Closed eye
    else if (auto eob = dynamic_cast<EyesOpenedBlock *>(block))
    {
        // 108 normal, 152 = hit (active animation state)
        if (eob->isHit())
            return 152;
        return 108; // Open eye
    }
    // Add more mappings as needed
    return 0;
}

int GameScreen::GetEnemyTileId(Enemy *enemy)
{
    if (dynamic_cast<Goomba *>(enemy))
        return 127;
    else if (dynamic_cast<GreenKoopa *>(enemy))
        return 128;
    else if (dynamic_cast<JumpingPiranhaPlant *>(enemy))
        return 129;
    else if (dynamic_cast<RedKoopa *>(enemy))
        return 134;
    else if (auto rex = dynamic_cast<Rex *>(enemy))
    {
        // 135 normal Rex, 173 = Rex after first hit (squashed once)
        if (rex->GetHitCount() >= 1)
            return 173;
        return 135;
    }
    else if (dynamic_cast<Bob_omb *>(enemy))
        return 123;
    else if (dynamic_cast<BuzzyBeetle *>(enemy))
        return 125;
    else if (dynamic_cast<FlyingGoomba *>(enemy))
        return 126;
    else if (dynamic_cast<BulletBill *>(enemy))
        return 124;
    else if (dynamic_cast<PiranhaPlant *>(enemy))
        return 133;
    else if (dynamic_cast<YellowKoopa *>(enemy))
        return 137;
    else if (dynamic_cast<BanzaiBill *>(enemy))
        return 145;
    return 0;
}

int GameScreen::GetItemTileId(Item *item)
{
    if (dynamic_cast<Coin *>(item))
        return 140;
    else if (dynamic_cast<Mushroom *>(item))
        return 142;
    else if (dynamic_cast<FireFlower *>(item))
        return 141;
    else if (dynamic_cast<Star *>(item))
        return 143;
    else if (dynamic_cast<OneUpMushroom *>(item))
        return 138;
    else if (dynamic_cast<ThreeUpMoon *>(item))
        return 139;
    else if (dynamic_cast<YoshiCoin *>(item))
        return 144;
    else if (dynamic_cast<CourseClearToken *>(item))
        return 146;
    return 0;
}