#pragma once
#include "Screen.h"

class ScreenController {
    private:
        Screen* currentState;
        Screen* menuState;
        Screen* map1State;
        Screen* map2State;
        bool shouldChangeState;
        StateType nextStateType;
        
    public:
        ScreenController();
        ~ScreenController();
        
        void Initialize();
        void Update();
        void Draw();
        void HandleInput();
        void ChangeState(StateType newState);
        void Cleanup();
        
        Screen* GetCurrentState() const { return currentState; }
        bool ShouldExit() const;
    };