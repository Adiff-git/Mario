#pragma once
#include "Screen.h"
#include "Button.h" 
#include "raylib.h"
#include <iostream>
#include <memory>
#include "Mario.h"
#include "GameWorld.h"

enum class TransitionState {
    NEXT_LEVEL,
    GAME_OVER,
    GAME_RESET,
    NONE
};

class GameScreen : public Screen {
    private:
        Button BackMenu;
        std::unique_ptr<GameWorld> gameWorld;

        int level;
        void NextLevel();
        TransitionState transitionState;
        float transitionTime;
        float transitionTimeAcum;

    public:
        GameScreen(ScreenController* screenController);
        void Update() override;
        void Draw() override;
        void ResetGame();
        void DrawEnd();
        void BeginTransition(TransitionState transitionState);

};