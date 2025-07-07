#pragma once
#include "Screen.h"
#include "Map.h"
class Map1Screen : public Screen {
    private:
        bool isPaused;
    public:
        Map1Screen();
        void Enter() override;
        void Update() override;
        void Draw() override;
        void Exit() override;
        void HandleInput() override;
};