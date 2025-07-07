#pragma once
#include "Screen.h"


class MenuScreen : public Screen {
    private:
        int selectedOption;
        int totalOptions;
        
    public:
        MenuScreen();
        void Enter() override;
        void Update() override;
        void Draw() override;
        void Exit() override;
        void HandleInput() override;
};