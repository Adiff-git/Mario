#pragma once
#include "Screen.h"
#include "Button.h"
#include "raylib.h"
#include <iostream>
class MenuScreen : public Screen {
    private:
        Button startButton;
        Button exitButton;
        Button settingsButton;
        Texture2D* backgroundTexture;
    public:
        MenuScreen(ScreenManager* screenManager);
        void Update() override;
        void Draw() override;
};