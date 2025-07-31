#pragma once
#include "Screen.h"
#include <stack>

class Screen;
class ScreenController {
private:
    std::stack<Screen*> screenStack;
public:
    ScreenController();
    ~ScreenController();
    void Update();
    void Draw();
    void ChangeScreen(Screen* newScreen); 
    void PushScreen(Screen* newScreen);   
    void PopScreen();                     
};