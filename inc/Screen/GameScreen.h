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

enum class TransitionState {
    NEXT_LEVEL,
    GAME_OVER,
    GAME_RESET,
    NONE
};
class GameScreen : public Screen {
    private:

    bool isPaused = false;

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
        
    public:
        GameScreen(ScreenController* screenController);
        GameScreen(ScreenController* screenController, bool multiplayer, CharacterType p1Type, CharacterType p2Type);
        void Update() override;
        void Draw() override;
        void ResetGame();
        void DrawEnd();
        void BeginTransition(TransitionState transitionState);

};