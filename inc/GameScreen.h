#pragma once
#include <memory>
#include "Screen.h"
#include "Button.h" 
#include "raylib.h"
#include <iostream>
#include "Character.h"
#include "GameWorld.h"
#include "GameHUD.h"
#include "ResrcManager.h"
#include "SoundManager.h"
#include "GameClock.h"
#include "SettingsScreen.h"

enum class TransitionState {
    NEXT_LEVEL,
    GAME_OVER,
    GAME_RESET,
    NONE
};
class GameScreen : public Screen {
    private:
        Button BackMenu;
        Button SettingsButton;
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
        
    public:
        GameScreen(ScreenController* screenController);
        GameScreen(ScreenController* screenController, bool multiplayer, CharacterType p1Type, CharacterType p2Type);
        void Update() override;
        void Draw() override;
        void ResetGame();
        void DrawEnd();
        void BeginTransition(TransitionState transitionState);

};