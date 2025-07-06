#pragma once
#include "GameState.h"
#include "Map.h"
class Map1State : public GameState {
    private:
        bool isPaused;
    public:
        Map1State();
        void Enter() override;
        void Update() override;
        void Draw() override;
        void Exit() override;
        void HandleInput() override;
};