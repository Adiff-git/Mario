#pragma once
#include "Screen.h"
#include "Button.h"
#include "CharacterSelectScreen.h"

enum class MapType {
    MAP_TUTORIAL,
    MAP_1,
    MAP_2,
    MAP_3,
    MAP_BOSS,
    NONE
};

enum class DifficultyLevel {
    EASY,
    MEDIUM,
    HARD,
    NONE
};

class MapSelectScreen : public Screen {
private:
    // Map selection buttons
    Button mapTutorialButton;
    Button map1Button;
    Button map2Button;
    Button map3Button;
    Button mapBossButton;
    
    // Difficulty buttons for each map
    Button easyButton1, mediumButton1, hardButton1;    // For Map 1
    Button easyButton2, mediumButton2, hardButton2;    // For Map 2
    Button easyButton3, mediumButton3, hardButton3;    // For Map 3
    
    Button backButton;
    Button startButton;
    
    // Game settings
    bool isMultiplayer;
    CharacterType player1Character;
    CharacterType player2Character;
    
    // Selection state
    MapType selectedMap;
    DifficultyLevel selectedDifficulty;
    bool mapSelected;
    bool difficultySelected;
    
    // UI textures
    Texture2D* backgroundTexture;
    Texture2D* selectMapTexture;
    Texture2D* selectDifficultyTexture;
    
    // Font
    Font* pixelFont;
    
public:
    MapSelectScreen(ScreenController* screenController, bool multiplayer, 
                   CharacterType p1Character, CharacterType p2Character);
    virtual ~MapSelectScreen();
    
    virtual void Update() override;
    virtual void Draw() override;
    
private:
    void SelectMap(MapType map);
    void SelectDifficulty(DifficultyLevel difficulty);
    bool CanStartGame();
    void UpdateDifficultyButtons();
};
