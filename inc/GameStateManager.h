#pragma once
#include "GameState.h"

class GameStateManager {
    private:
        GameState* currentState;
        GameState* menuState;
        GameState* map1State;
        GameState* map2State;
        bool shouldChangeState;
        StateType nextStateType;
        
    public:
        GameStateManager();
        ~GameStateManager();
        
        void Initialize();
        void Update();
        void Draw();
        void HandleInput();
        void ChangeState(StateType newState);
        void Cleanup();
        
        GameState* GetCurrentState() const { return currentState; }
        bool ShouldExit() const;
    };