#pragma once
#include "Screen.h"
#include "Button.h"
#include "raylib.h"
#include <iostream>
class ScreenController; // Forward declaration
class MenuScreen : public Screen {
    private:
        Button startButton;
        Button MapEditorButton;
        Button ContinueButton;
        // Button exitButton;
        Button settingsButton;
        Texture2D* backgroundTexture;
        Texture2D* logoTexture;
    public:
        MenuScreen(ScreenController* screenController);
        void Update() override;
        void Draw() override;
};