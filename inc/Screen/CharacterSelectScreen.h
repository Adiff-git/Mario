#pragma once
#include "Screen.h"
#include "Button.h"
#include "../inc/Character/CharacterType.h"



class CharacterSelectScreen : public Screen {
private:
    Button marioButton;
    Button luigiButton;
    Button toadButton;
    Button peachButton;
    Button backButton;
    Button startButton;
    
    bool isMultiplayer;
    CharacterType player1Character;
    CharacterType player2Character;
    bool player1Selected;
    bool player2Selected;
    
    Texture2D* backgroundTexture;
    Texture2D* chooseCharacterTexture;
    
public:
    CharacterSelectScreen(ScreenController* screenController, bool multiplayer);
    virtual ~CharacterSelectScreen();
    
    virtual void Update() override;
    virtual void Draw() override;
    
private:
    void SelectCharacter(CharacterType character);
    bool CanStartGame();
};