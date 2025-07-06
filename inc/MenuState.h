#pragma once
#include "GameState.h"


class MenuState : public GameState {
    private:
        int selectedOption;
        int totalOptions;
        
    public:
        MenuState();
        void Enter() override;
        void Update() override;
        void Draw() override;
        void Exit() override;
        void HandleInput() override;
};