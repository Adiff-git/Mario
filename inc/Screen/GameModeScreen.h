#pragma once
#include "Screen.h"
#include "Button.h"

class GameModeScreen : public Screen {
private:
    Texture2D* backgroundTexture;
    Texture2D* choosePlayerTexture;
    Button singlePlayerButton;
    Button multiPlayerButton;
    Button backButton;
    
    // Hover state tracking
    bool singlePlayerHovered;
    bool multiPlayerHovered;
    
public:
    GameModeScreen(ScreenController* screenController);
    virtual ~GameModeScreen();
    
    virtual void Update() override;
    virtual void Draw() override;
};