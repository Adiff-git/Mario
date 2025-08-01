#pragma once
#include "Screen.h"

class Screen;
class ScreenController {
    private:
        Screen* currentScreen;
//unique_pointer<Screen> Cúcreen;
    public:
        ScreenController();
        ~ScreenController();
        void Update();
        void Draw();
        void ChangeScreen(Screen* newScreen);
};