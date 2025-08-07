#pragma once
#include "Screen.h"
#include "Button.h"

enum class CharacterType {
    MARIO,
    LUIGI,
    TOAD,
    PEACH,
    NONE
};

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
    Font* pixelFont;
    
    // Hover state tracking
    bool marioHovered;
    bool luigiHovered;
    bool toadHovered;
    bool peachHovered;
    
    // Scale for selected characters
    float marioScale;
    float luigiScale;
    float toadScale;
    float peachScale;
    
    // Track which player is making the next selection
    bool nextSelectionForPlayer1;
    
public:
    CharacterSelectScreen(ScreenController* screenController, bool multiplayer);
    virtual ~CharacterSelectScreen();
    
    virtual void Update() override;
    virtual void Draw() override;
    
private:
    void SelectCharacter(CharacterType character);
    bool CanStartGame();
};