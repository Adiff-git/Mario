#pragma once
#include <memory>
#include "Screen.h"
#include "Button.h" 
#include "raylib.h"
#include <iostream>
#include "../inc/Character/Character.h"
#include "../inc/World/GameWorld.h"
#include "../inc/World/GameHUD.h"
#include "ResrcManager.h"
#include "SoundManager.h"
#include "../inc/World/GameClock.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>
// Forward declarations to avoid circular includes
enum class MapType;
enum class DifficultyLevel;

enum class TransitionState {
    NEXT_LEVEL,
    GAME_OVER,
    GAME_RESET,
    NONE
};
class GameScreen : public Screen {
    private:

        int PauseScreenWidth = 500;
        int PauseScreenHeight = 400;
        int PauseScreenPosX = GetScreenWidth()/2 - PauseScreenWidth/2;
        int PauseScreenPosY = GetScreenHeight()/2 - PauseScreenHeight/2;
        Button BackMenu;
        std::unique_ptr<GameWorld> gameWorld;
        std::unique_ptr<GameHUD> gameHUD;
        int level;
        void NextLevel();
        TransitionState transitionState;
        float transitionTime;
        float transitionTimeAcum;

        bool isMultiplayer;
        CharacterType player1Type;
        CharacterType player2Type;
        MapType selectedMap;
        DifficultyLevel selectedDifficulty;

        Button PlayButton;
        Button MenuButton;
        Button SaveButton;

        bool isPaused = false;
        bool showPauseMenu = false;
        int pauseMusicVolume ;
        int pauseSfxVolume ;
        bool requestGoHome = false;

        // Save dialog variables
        bool showSaveDialog = false;
        char saveFileName[64] = {0};
        int saveNameLength = 0;
        
    public:
        GameScreen(ScreenController* screenController);
        GameScreen(ScreenController* screenController, bool multiplayer, CharacterType p1Type, CharacterType p2Type);
        GameScreen(ScreenController* screenController, bool multiplayer, CharacterType p1Type, CharacterType p2Type, 
                  MapType map, DifficultyLevel difficulty);
    // Load from a saved map json path (resources/save/*.json)
    GameScreen(ScreenController* screenController, const std::string& savedMapPath);
        void Update() override;
        void Draw() override;
        void ResetGame();
        void DrawEnd();
        void BeginTransition(TransitionState transitionState);

        std::string GetCurrentDateTime();
        void SaveMapInSettings(const std::string& fileName = "");
        int GetBlockTileId(Block *block);
        int GetEnemyTileId(Enemy *enemy);
        int GetItemTileId(Item *item);

    private:
        std::string loadedSavePath; // non-empty if constructed from saved map file



};