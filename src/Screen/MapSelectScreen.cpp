#include "../inc/Screen/MapSelectScreen.h"
#include "../inc/Screen/CharacterSelectScreen.h"
#include "../inc/Screen/GameScreen.h"
#include "ResrcManager.h"
#include "SoundManager.h"
#include <cstdio>

MapSelectScreen::MapSelectScreen(ScreenController* screenController, bool multiplayer, 
                               CharacterType p1Character, CharacterType p2Character)
    : Screen(screenController),
      isMultiplayer(multiplayer),
      player1Character(p1Character),
      player2Character(p2Character),
      selectedMap(MapType::NONE),
      selectedDifficulty(DifficultyLevel::NONE),
      mapSelected(false),
      difficultySelected(false),
      // Map selection buttons (horizontal layout)
      map1Button(Vector2{(float)GetScreenWidth()/2 - 400, (float)GetScreenHeight()/2 - 200}, Vector2{200, 150}),
      map2Button(Vector2{(float)GetScreenWidth()/2 - 100, (float)GetScreenHeight()/2 - 200}, Vector2{200, 150}),
      map3Button(Vector2{(float)GetScreenWidth()/2 + 200, (float)GetScreenHeight()/2 - 200}, Vector2{200, 150}),
      // Difficulty buttons for Map 1
      easyButton1(Vector2{(float)GetScreenWidth()/2 - 450, (float)GetScreenHeight()/2 + 50}, Vector2{100, 60}),
      mediumButton1(Vector2{(float)GetScreenWidth()/2 - 350, (float)GetScreenHeight()/2 + 50}, Vector2{100, 60}),
      hardButton1(Vector2{(float)GetScreenWidth()/2 - 250, (float)GetScreenHeight()/2 + 50}, Vector2{100, 60}),
      // Difficulty buttons for Map 2
      easyButton2(Vector2{(float)GetScreenWidth()/2 - 150, (float)GetScreenHeight()/2 + 50}, Vector2{100, 60}),
      mediumButton2(Vector2{(float)GetScreenWidth()/2 - 50, (float)GetScreenHeight()/2 + 50}, Vector2{100, 60}),
      hardButton2(Vector2{(float)GetScreenWidth()/2 + 50, (float)GetScreenHeight()/2 + 50}, Vector2{100, 60}),
      // Difficulty buttons for Map 3
      easyButton3(Vector2{(float)GetScreenWidth()/2 + 150, (float)GetScreenHeight()/2 + 50}, Vector2{100, 60}),
      mediumButton3(Vector2{(float)GetScreenWidth()/2 + 250, (float)GetScreenHeight()/2 + 50}, Vector2{100, 60}),
      hardButton3(Vector2{(float)GetScreenWidth()/2 + 350, (float)GetScreenHeight()/2 + 50}, Vector2{100, 60}),
      // Control buttons
      backButton(Vector2{50, 50}, Vector2{80, 80}),
      startButton(Vector2{(float)GetScreenWidth()/2 - 100, (float)GetScreenHeight()/2 + 200}, Vector2{200, 80})
{
    // Load textures
    backgroundTexture = &ResrcManager::GetInstance().getTexture("BACKGROUND_10");
    selectMapTexture = &ResrcManager::GetInstance().getTexture("SELECT_MAP");
    selectDifficultyTexture = &ResrcManager::GetInstance().getTexture("SELECT_DIFFICULTY");
    
    // Set map button textures
    map1Button.SetTexture(ResrcManager::GetInstance().getTexture("MAP_1"));
    map2Button.SetTexture(ResrcManager::GetInstance().getTexture("MAP_2"));
    map3Button.SetTexture(ResrcManager::GetInstance().getTexture("MAP_3"));
    
    // Set difficulty button textures
    easyButton1.SetTexture(ResrcManager::GetInstance().getTexture("EASY_BUTTON"));
    mediumButton1.SetTexture(ResrcManager::GetInstance().getTexture("MEDIUM_BUTTON"));
    hardButton1.SetTexture(ResrcManager::GetInstance().getTexture("HARD_BUTTON"));
    
    easyButton2.SetTexture(ResrcManager::GetInstance().getTexture("EASY_BUTTON"));
    mediumButton2.SetTexture(ResrcManager::GetInstance().getTexture("MEDIUM_BUTTON"));
    hardButton2.SetTexture(ResrcManager::GetInstance().getTexture("HARD_BUTTON"));
    
    easyButton3.SetTexture(ResrcManager::GetInstance().getTexture("EASY_BUTTON"));
    mediumButton3.SetTexture(ResrcManager::GetInstance().getTexture("MEDIUM_BUTTON"));
    hardButton3.SetTexture(ResrcManager::GetInstance().getTexture("HARD_BUTTON"));
    
    // Set control button textures
    backButton.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
    startButton.SetTexture(ResrcManager::GetInstance().getTexture("START_BUTTON"));
}

MapSelectScreen::~MapSelectScreen() {
}

void MapSelectScreen::Update() {
    // Update all buttons
    map1Button.Update();
    map2Button.Update();
    map3Button.Update();
    
    UpdateDifficultyButtons();
    
    backButton.Update();
    startButton.Update();
    
    // Map selection logic
    if (map1Button.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        SelectMap(MapType::MAP_1);
    }
    
    if (map2Button.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        SelectMap(MapType::MAP_2);
    }
    
    if (map3Button.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        SelectMap(MapType::MAP_3);
    }
    
    // Difficulty selection logic (only if map is selected)
    if (mapSelected) {
        if (selectedMap == MapType::MAP_1) {
            if (easyButton1.IsPressed()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::EASY);
            }
            if (mediumButton1.IsPressed()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::MEDIUM);
            }
            if (hardButton1.IsPressed()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::HARD);
            }
        } else if (selectedMap == MapType::MAP_2) {
            if (easyButton2.IsPressed()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::EASY);
            }
            if (mediumButton2.IsPressed()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::MEDIUM);
            }
            if (hardButton2.IsPressed()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::HARD);
            }
        } else if (selectedMap == MapType::MAP_3) {
            if (easyButton3.IsPressed()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::EASY);
            }
            if (mediumButton3.IsPressed()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::MEDIUM);
            }
            if (hardButton3.IsPressed()) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::HARD);
            }
        }
    }
    
    // Start game if both map and difficulty are selected
    if (startButton.IsPressed() && CanStartGame()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        // Pass map and difficulty information to GameScreen
        screenController->ChangeScreen(new GameScreen(screenController, isMultiplayer, 
                                                     player1Character, player2Character, 
                                                     selectedMap, selectedDifficulty));
    }
    
    // Back to character selection
    if (backButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        screenController->ChangeScreen(new CharacterSelectScreen(screenController, isMultiplayer));
    }
}

void MapSelectScreen::SelectMap(MapType map) {
    selectedMap = map;
    mapSelected = true;
    // Reset difficulty selection when map changes
    selectedDifficulty = DifficultyLevel::NONE;
    difficultySelected = false;
}

void MapSelectScreen::SelectDifficulty(DifficultyLevel difficulty) {
    selectedDifficulty = difficulty;
    difficultySelected = true;
}

bool MapSelectScreen::CanStartGame() {
    return mapSelected && difficultySelected;
}

void MapSelectScreen::UpdateDifficultyButtons() {
    // Only update difficulty buttons if a map is selected
    if (mapSelected) {
        if (selectedMap == MapType::MAP_1) {
            easyButton1.Update();
            mediumButton1.Update();
            hardButton1.Update();
        } else if (selectedMap == MapType::MAP_2) {
            easyButton2.Update();
            mediumButton2.Update();
            hardButton2.Update();
        } else if (selectedMap == MapType::MAP_3) {
            easyButton3.Update();
            mediumButton3.Update();
            hardButton3.Update();
        }
    }
}

void MapSelectScreen::Draw() {
    // Draw background
    DrawTexturePro(*backgroundTexture, 
                   Rectangle{0, 0, (float)backgroundTexture->width, (float)backgroundTexture->height},
                   Rectangle{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                   Vector2{0, 0}, 0.0f, WHITE);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.3f));
    
    // Draw title
    DrawText("SELECT MAP & DIFFICULTY", GetScreenWidth()/2 - 250, 100, 40, WHITE);
    
    // Draw map selection title
    DrawText("Choose a Map:", GetScreenWidth()/2 - 120, GetScreenHeight()/2 - 280, 30, YELLOW);
    
    // Draw map buttons
    map1Button.Draw();
    map2Button.Draw();
    map3Button.Draw();
    
    // Draw map names
    DrawText("Map 1", GetScreenWidth()/2 - 330, GetScreenHeight()/2 - 30, 20, WHITE);
    DrawText("Map 2", GetScreenWidth()/2 - 30, GetScreenHeight()/2 - 30, 20, WHITE);
    DrawText("Map 3", GetScreenWidth()/2 + 270, GetScreenHeight()/2 - 30, 20, WHITE);
    
    // Draw map selection indicator
    if (mapSelected) {
        Color selectionColor = GREEN;
        float selectionX = 0;
        switch(selectedMap) {
            case MapType::MAP_1: selectionX = (float)GetScreenWidth()/2 - 400; break;
            case MapType::MAP_2: selectionX = (float)GetScreenWidth()/2 - 100; break;
            case MapType::MAP_3: selectionX = (float)GetScreenWidth()/2 + 200; break;
        }
        DrawRectangleLines(selectionX - 5, (float)GetScreenHeight()/2 - 205, 210, 160, selectionColor);
        DrawRectangleLines(selectionX - 3, (float)GetScreenHeight()/2 - 203, 206, 156, selectionColor);
        
        // Draw difficulty selection title
        DrawText("Choose Difficulty:", GetScreenWidth()/2 - 140, GetScreenHeight()/2 + 10, 25, YELLOW);
        
        // Draw difficulty buttons for selected map
        if (selectedMap == MapType::MAP_1) {
            easyButton1.Draw();
            mediumButton1.Draw();
            hardButton1.Draw();
            DrawText("Easy", GetScreenWidth()/2 - 430, GetScreenHeight()/2 + 120, 16, WHITE);
            DrawText("Medium", GetScreenWidth()/2 - 340, GetScreenHeight()/2 + 120, 16, WHITE);
            DrawText("Hard", GetScreenWidth()/2 - 230, GetScreenHeight()/2 + 120, 16, WHITE);
        } else if (selectedMap == MapType::MAP_2) {
            easyButton2.Draw();
            mediumButton2.Draw();
            hardButton2.Draw();
            DrawText("Easy", GetScreenWidth()/2 - 130, GetScreenHeight()/2 + 120, 16, WHITE);
            DrawText("Medium", GetScreenWidth()/2 - 40, GetScreenHeight()/2 + 120, 16, WHITE);
            DrawText("Hard", GetScreenWidth()/2 + 70, GetScreenHeight()/2 + 120, 16, WHITE);
        } else if (selectedMap == MapType::MAP_3) {
            easyButton3.Draw();
            mediumButton3.Draw();
            hardButton3.Draw();
            DrawText("Easy", GetScreenWidth()/2 + 170, GetScreenHeight()/2 + 120, 16, WHITE);
            DrawText("Medium", GetScreenWidth()/2 + 260, GetScreenHeight()/2 + 120, 16, WHITE);
            DrawText("Hard", GetScreenWidth()/2 + 370, GetScreenHeight()/2 + 120, 16, WHITE);
        }
        
        // Draw difficulty selection indicator
        if (difficultySelected) {
            Color difficultyColor = ORANGE;
            float difficultyX = 0;
            
            if (selectedMap == MapType::MAP_1) {
                switch(selectedDifficulty) {
                    case DifficultyLevel::EASY: difficultyX = (float)GetScreenWidth()/2 - 450; break;
                    case DifficultyLevel::MEDIUM: difficultyX = (float)GetScreenWidth()/2 - 350; break;
                    case DifficultyLevel::HARD: difficultyX = (float)GetScreenWidth()/2 - 250; break;
                }
            } else if (selectedMap == MapType::MAP_2) {
                switch(selectedDifficulty) {
                    case DifficultyLevel::EASY: difficultyX = (float)GetScreenWidth()/2 - 150; break;
                    case DifficultyLevel::MEDIUM: difficultyX = (float)GetScreenWidth()/2 - 50; break;
                    case DifficultyLevel::HARD: difficultyX = (float)GetScreenWidth()/2 + 50; break;
                }
            } else if (selectedMap == MapType::MAP_3) {
                switch(selectedDifficulty) {
                    case DifficultyLevel::EASY: difficultyX = (float)GetScreenWidth()/2 + 150; break;
                    case DifficultyLevel::MEDIUM: difficultyX = (float)GetScreenWidth()/2 + 250; break;
                    case DifficultyLevel::HARD: difficultyX = (float)GetScreenWidth()/2 + 350; break;
                }
            }
            
            DrawRectangleLines(difficultyX - 3, (float)GetScreenHeight()/2 + 47, 106, 66, difficultyColor);
            DrawRectangleLines(difficultyX - 1, (float)GetScreenHeight()/2 + 49, 102, 62, difficultyColor);
        }
    }
    
    // Draw control buttons
    if (CanStartGame()) {
        startButton.Draw();
    }
    
    backButton.Draw();
    
    // Draw instructions
    if (!mapSelected) {
        DrawText("Please select a map first", GetScreenWidth()/2 - 150, GetScreenHeight() - 100, 20, RED);
    } else if (!difficultySelected) {
        DrawText("Please select a difficulty level", GetScreenWidth()/2 - 170, GetScreenHeight() - 100, 20, RED);
    } else {
        DrawText("Press START to begin the game!", GetScreenWidth()/2 - 160, GetScreenHeight() - 100, 20, GREEN);
        
        // Display speed information
        float baseSpeed = 1.0f, levelSpeed = 1.0f;
        switch(selectedDifficulty) {
            case DifficultyLevel::EASY: baseSpeed = 0.6f; break;
            case DifficultyLevel::MEDIUM: baseSpeed = 1.0f; break;
            case DifficultyLevel::HARD: baseSpeed = 1.8f; break;
        }
        switch(selectedMap) {
            case MapType::MAP_1: levelSpeed = 1.0f; break;
            case MapType::MAP_2: levelSpeed = 2.0f; break;
            case MapType::MAP_3: levelSpeed = 3.5f; break;
        }
        float totalSpeed = baseSpeed * levelSpeed;
        
        char speedInfo[100];
        sprintf(speedInfo, "Enemy Speed: %.1fx (Difficulty: %.1fx, Level: %.1fx)", totalSpeed, baseSpeed, levelSpeed);
        DrawText(speedInfo, GetScreenWidth()/2 - 200, GetScreenHeight() - 70, 16, YELLOW);
    }
}
