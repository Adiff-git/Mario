#include "../inc/Screen/MapSelectScreen.h"
#include "../inc/Screen/CharacterSelectScreen.h"
#include "../inc/Screen/GameScreen.h"
#include "ResrcManager.h"
#include "SoundManager.h"
#include <cstdio>

MapSelectScreen::MapSelectScreen(ScreenController *screenController, bool multiplayer,
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
    mapTutorialButton(Vector2{(float)GetScreenWidth() / 2 - 650, (float)GetScreenHeight() / 2 - 200}, Vector2{200, 150}),
    map1Button(Vector2{(float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 200}, Vector2{200, 150}),
    map2Button(Vector2{(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 200}, Vector2{200, 150}),
    map3Button(Vector2{(float)GetScreenWidth() / 2 + 200, (float)GetScreenHeight() / 2 - 200}, Vector2{200, 150}),
    mapBossButton(Vector2{(float)GetScreenWidth() / 2 + 450, (float)GetScreenHeight() / 2 - 200}, Vector2{200, 150}),
      // Difficulty buttons for Map 1
      easyButton1(Vector2{(float)GetScreenWidth() / 2 - 450, (float)GetScreenHeight() / 2 + 50}, Vector2{30, 20}),
      mediumButton1(Vector2{(float)GetScreenWidth() / 2 - 350, (float)GetScreenHeight() / 2 + 50}, Vector2{60, 40}),
      hardButton1(Vector2{(float)GetScreenWidth() / 2 - 250, (float)GetScreenHeight() / 2 + 50}, Vector2{100, 60}),
      // Difficulty buttons for Map 2
      easyButton2(Vector2{(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 50}, Vector2{30, 20}),
      mediumButton2(Vector2{(float)GetScreenWidth() / 2 - 50, (float)GetScreenHeight() / 2 + 50}, Vector2{60, 40}),
      hardButton2(Vector2{(float)GetScreenWidth() / 2 + 50, (float)GetScreenHeight() / 2 + 50}, Vector2{100, 60}),
      // Difficulty buttons for Map 3
      easyButton3(Vector2{(float)GetScreenWidth() / 2 + 150, (float)GetScreenHeight() / 2 + 50}, Vector2{30, 20}),
      mediumButton3(Vector2{(float)GetScreenWidth() / 2 + 250, (float)GetScreenHeight() / 2 + 50}, Vector2{60, 40}),
      hardButton3(Vector2{(float)GetScreenWidth() / 2 + 350, (float)GetScreenHeight() / 2 + 50}, Vector2{100, 60}),
      // Control buttons
      backButton(Vector2{50, 50}, Vector2{80, 80}),
      startButton(Vector2{(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 200}, Vector2{200, 80})
{
    // Load textures
    backgroundTexture = &ResrcManager::GetInstance().getTexture("BACKGROUND_10");
    selectMapTexture = &ResrcManager::GetInstance().getTexture("SELECT_MAP");
    selectDifficultyTexture = &ResrcManager::GetInstance().getTexture("SELECT_DIFFICULTY");

    // Load font
    pixelFont = &ResrcManager::GetInstance().getFont("SUPER_MARIO_WORLD_FONT");

    // Set map button textures
    mapTutorialButton.SetTexture(ResrcManager::GetInstance().getTexture("MAP_TUTORIAL"));
    map1Button.SetTexture(ResrcManager::GetInstance().getTexture("MAP_1"));
    map2Button.SetTexture(ResrcManager::GetInstance().getTexture("MAP_2"));
    map3Button.SetTexture(ResrcManager::GetInstance().getTexture("MAP_3"));
    mapBossButton.SetTexture(ResrcManager::GetInstance().getTexture("MAP_BOSS"));

    // Set difficulty button textures
    easyButton1.SetTexture(ResrcManager::GetInstance().getTexture("LEVEL 1"));
    mediumButton1.SetTexture(ResrcManager::GetInstance().getTexture("LEVEL 2"));
    hardButton1.SetTexture(ResrcManager::GetInstance().getTexture("LEVEL 3"));

    easyButton2.SetTexture(ResrcManager::GetInstance().getTexture("LEVEL 1"));
    mediumButton2.SetTexture(ResrcManager::GetInstance().getTexture("LEVEL 2"));
    hardButton2.SetTexture(ResrcManager::GetInstance().getTexture("LEVEL 3"));

    easyButton3.SetTexture(ResrcManager::GetInstance().getTexture("LEVEL 1"));
    mediumButton3.SetTexture(ResrcManager::GetInstance().getTexture("LEVEL 2"));
    hardButton3.SetTexture(ResrcManager::GetInstance().getTexture("LEVEL 3"));

    // Set control button textures
    backButton.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
    startButton.SetTexture(ResrcManager::GetInstance().getTexture("START_BUTTON"));
}

MapSelectScreen::~MapSelectScreen()
{
}

void MapSelectScreen::Update()
{
    // Update all buttons
    mapTutorialButton.Update();
    map1Button.Update();
    map2Button.Update();
    map3Button.Update();
    mapBossButton.Update();

    UpdateDifficultyButtons();

    backButton.Update();
    startButton.Update();

    // Map selection logic
    if (mapTutorialButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        SelectMap(MapType::MAP_TUTORIAL);
    }
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
    if (mapBossButton.IsPressed()) {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        SelectMap(MapType::MAP_BOSS);
    }

    // Difficulty selection logic (only if map is selected)
    if (mapSelected && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (selectedMap == MapType::MAP_TUTORIAL || selectedMap == MapType::MAP_BOSS) {
            // Tutorial và Boss không có chọn độ khó
            SelectDifficulty(DifficultyLevel::EASY);
        } else if (selectedMap == MapType::MAP_1) {
            Rectangle easyRect = {(float)GetScreenWidth() / 2 - 450 + 25, (float)GetScreenHeight() / 2 + 30 + (60-20), 30, 20};
            Rectangle mediumRect = {(float)GetScreenWidth() / 2 - 350 + 25, (float)GetScreenHeight() / 2 + 30 + (60-40), 60, 40};
            Rectangle hardRect = {(float)GetScreenWidth() / 2 - 250 + 25, (float)GetScreenHeight() / 2 + 30, 100, 60};
            if (CheckCollisionPointRec(mousePos, easyRect)) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::EASY);
            } else if (CheckCollisionPointRec(mousePos, mediumRect)) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::MEDIUM);
            } else if (CheckCollisionPointRec(mousePos, hardRect)) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::HARD);
            }
        } else if (selectedMap == MapType::MAP_2) {
            Rectangle easyRect = {(float)GetScreenWidth() / 2 - 150 + 25, (float)GetScreenHeight() / 2 + 30 + (60-20), 30, 20};
            Rectangle mediumRect = {(float)GetScreenWidth() / 2 - 50 + 25, (float)GetScreenHeight() / 2 + 30 + (60-40), 60, 40};
            Rectangle hardRect = {(float)GetScreenWidth() / 2 + 50 + 25, (float)GetScreenHeight() / 2 + 30, 100, 60};
            if (CheckCollisionPointRec(mousePos, easyRect)) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::EASY);
            } else if (CheckCollisionPointRec(mousePos, mediumRect)) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::MEDIUM);
            } else if (CheckCollisionPointRec(mousePos, hardRect)) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::HARD);
            }
        } else if (selectedMap == MapType::MAP_3) {
            Rectangle easyRect = {(float)GetScreenWidth() / 2 + 150 + 25, (float)GetScreenHeight() / 2 + 30 + (60-20), 30, 20};
            Rectangle mediumRect = {(float)GetScreenWidth() / 2 + 250 + 25, (float)GetScreenHeight() / 2 + 30 + (60-40), 60, 40};
            Rectangle hardRect = {(float)GetScreenWidth() / 2 + 350 + 25, (float)GetScreenHeight() / 2 + 30, 100, 60};
            if (CheckCollisionPointRec(mousePos, easyRect)) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::EASY);
            } else if (CheckCollisionPointRec(mousePos, mediumRect)) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::MEDIUM);
            } else if (CheckCollisionPointRec(mousePos, hardRect)) {
                SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
                SelectDifficulty(DifficultyLevel::HARD);
            }
        }
    }

    // Start game if both map and difficulty are selected
    if (startButton.IsPressed() && CanStartGame())
    {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        // Pass map and difficulty information to GameScreen
        screenController->ChangeScreen(new GameScreen(screenController, isMultiplayer,
                                                      player1Character, player2Character,
                                                      selectedMap, selectedDifficulty));
    }

    // Back to character selection
    if (backButton.IsPressed())
    {
        SoundManager::GetInstance().PlaySound("BUTTON_CLICK");
        screenController->ChangeScreen(new CharacterSelectScreen(screenController, isMultiplayer));
    }
}

void MapSelectScreen::SelectMap(MapType map)
{
    selectedMap = map;
    mapSelected = true;
    // Reset difficulty selection when map changes
    selectedDifficulty = DifficultyLevel::NONE;
    difficultySelected = false;
}

void MapSelectScreen::SelectDifficulty(DifficultyLevel difficulty)
{
    selectedDifficulty = difficulty;
    difficultySelected = true;
}

bool MapSelectScreen::CanStartGame()
{
    return mapSelected && difficultySelected;
}

void MapSelectScreen::UpdateDifficultyButtons()
{
    // No longer needed - difficulty selection is handled directly in Update()
}

void MapSelectScreen::Draw()
{
    // Draw background
    DrawTexturePro(*backgroundTexture,
                   Rectangle{0, 0, (float)backgroundTexture->width, (float)backgroundTexture->height},
                   Rectangle{0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                   Vector2{0, 0}, 0.0f, WHITE);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.3f));

    // Draw main title - centered
    const char *mainTitle = "SELECT MAP & DIFFICULTY";
    Vector2 titleSize = MeasureTextEx(*pixelFont, mainTitle, 36, 2);
    DrawTextEx(*pixelFont, mainTitle,
               Vector2{(float)GetScreenWidth() / 2 - titleSize.x / 2, 80},
               36, 2, WHITE);

    // Draw map selection title - centered
    const char *mapTitle = "Choose a Map:";
    Vector2 mapTitleSize = MeasureTextEx(*pixelFont, mapTitle, 24, 2);
    DrawTextEx(*pixelFont, mapTitle,
               Vector2{(float)GetScreenWidth() / 2 - mapTitleSize.x / 2, (float)GetScreenHeight() / 2 - 280},
               24, 2, YELLOW);

    // Draw map buttons
    mapTutorialButton.Draw();
    map1Button.Draw();
    map2Button.Draw();
    map3Button.Draw();
    mapBossButton.Draw();

    // Draw map names - centered under each button
    const char *mapTutorialText = "Tutorial";
    const char *map1Text = "Map 1";
    const char *map2Text = "Map 2";
    const char *map3Text = "Map 3";
    const char *mapBossText = "Boss";
    Vector2 mapTutorialSize = MeasureTextEx(*pixelFont, mapTutorialText, 18, 1);
    Vector2 map1Size = MeasureTextEx(*pixelFont, map1Text, 18, 1);
    Vector2 map2Size = MeasureTextEx(*pixelFont, map2Text, 18, 1);
    Vector2 map3Size = MeasureTextEx(*pixelFont, map3Text, 18, 1);
    Vector2 mapBossSize = MeasureTextEx(*pixelFont, mapBossText, 18, 1);

    DrawTextEx(*pixelFont, mapTutorialText,
               Vector2{(float)GetScreenWidth() / 2 - 650 + 100 - mapTutorialSize.x / 2, (float)GetScreenHeight() / 2 - 30},
               18, 1, WHITE);
    DrawTextEx(*pixelFont, map1Text,
               Vector2{(float)GetScreenWidth() / 2 - 400 + 100 - map1Size.x / 2, (float)GetScreenHeight() / 2 - 30},
               18, 1, WHITE);
    DrawTextEx(*pixelFont, map2Text,
               Vector2{(float)GetScreenWidth() / 2 - 100 + 100 - map2Size.x / 2, (float)GetScreenHeight() / 2 - 30},
               18, 1, WHITE);
    DrawTextEx(*pixelFont, map3Text,
               Vector2{(float)GetScreenWidth() / 2 + 200 + 100 - map3Size.x / 2, (float)GetScreenHeight() / 2 - 30},
               18, 1, WHITE);
    DrawTextEx(*pixelFont, mapBossText,
               Vector2{(float)GetScreenWidth() / 2 + 450 + 100 - mapBossSize.x / 2, (float)GetScreenHeight() / 2 - 30},
               18, 1, WHITE);

    // Draw map selection indicator
    if (mapSelected)
    {
        Color selectionColor = GREEN;
        float selectionX = 0;
        switch (selectedMap)
        {
        case MapType::MAP_TUTORIAL:
            selectionX = (float)GetScreenWidth() / 2 - 650;
            break;
        case MapType::MAP_1:
            selectionX = (float)GetScreenWidth() / 2 - 400;
            break;
        case MapType::MAP_2:
            selectionX = (float)GetScreenWidth() / 2 - 100;
            break;
        case MapType::MAP_3:
            selectionX = (float)GetScreenWidth() / 2 + 200;
            break;
        case MapType::MAP_BOSS:
            selectionX = (float)GetScreenWidth() / 2 + 450;
            break;
        }
        DrawRectangleLines(selectionX - 5, (float)GetScreenHeight() / 2 - 205, 210, 160, selectionColor);
        DrawRectangleLines(selectionX - 3, (float)GetScreenHeight() / 2 - 203, 206, 156, selectionColor);

        // Draw difficulty selection title - centered
        const char *difficultyTitle = "Choose Difficulty:";
        Vector2 difficultyTitleSize = MeasureTextEx(*pixelFont, difficultyTitle, 20, 1);
        DrawTextEx(*pixelFont, difficultyTitle,
                   Vector2{(float)GetScreenWidth() / 2 - difficultyTitleSize.x / 2, (float)GetScreenHeight() / 2 + 10},
                   20, 1, YELLOW);

        // Draw difficulty buttons for selected map
        if (selectedMap == MapType::MAP_1)
        {
            // Draw level textures as buttons using constructor sizes - aligned to bottom
            Rectangle easyRect = {(float)GetScreenWidth() / 2 - 450 + 25, (float)GetScreenHeight() / 2 + 30 + (60-20), 30, 20};
            Rectangle mediumRect = {(float)GetScreenWidth() / 2 - 350 + 25, (float)GetScreenHeight() / 2 + 30 + (60-40), 60, 40};
            Rectangle hardRect = {(float)GetScreenWidth() / 2 - 250 + 25, (float)GetScreenHeight() / 2 + 30, 100, 60};

            // Draw selection highlight if this difficulty is selected
            if (difficultySelected)
            {
                Rectangle highlightRect = {0, 0, 100, 60}; // Fixed size for all difficulty highlights
                switch (selectedDifficulty)
                {
                case DifficultyLevel::EASY:
                    highlightRect.x = (float)GetScreenWidth() / 2 - 450 + 25;
                    highlightRect.y = (float)GetScreenHeight() / 2 + 30;
                    break;
                case DifficultyLevel::MEDIUM:
                    highlightRect.x = (float)GetScreenWidth() / 2 - 350 + 25;
                    highlightRect.y = (float)GetScreenHeight() / 2 + 30;
                    break;
                case DifficultyLevel::HARD:
                    highlightRect.x = (float)GetScreenWidth() / 2 - 250 + 25;
                    highlightRect.y = (float)GetScreenHeight() / 2 + 30;
                    break;
                }
                DrawRectangleLines(highlightRect.x - 3, highlightRect.y - 3, highlightRect.width + 6, highlightRect.height + 6, ORANGE);
                DrawRectangleLines(highlightRect.x - 1, highlightRect.y - 1, highlightRect.width + 2, highlightRect.height + 2, ORANGE);
            }

            // Check if textures exist before drawing
            Texture2D &level1Tex = ResrcManager::GetInstance().getTexture("LEVEL 1");
            Texture2D &level2Tex = ResrcManager::GetInstance().getTexture("LEVEL 2");
            Texture2D &level3Tex = ResrcManager::GetInstance().getTexture("LEVEL 3");

            if (level1Tex.id != 0)
            {
                DrawTexturePro(level1Tex,
                               Rectangle{0, 0, (float)level1Tex.width, (float)level1Tex.height},
                               easyRect, Vector2{0, 0}, 0.0f, WHITE);
            }
            if (level2Tex.id != 0)
            {
                DrawTexturePro(level2Tex,
                               Rectangle{0, 0, (float)level2Tex.width, (float)level2Tex.height},
                               mediumRect, Vector2{0, 0}, 0.0f, WHITE);
            }
            if (level3Tex.id != 0)
            {
                DrawTexturePro(level3Tex,
                               Rectangle{0, 0, (float)level3Tex.width, (float)level3Tex.height},
                               hardRect, Vector2{0, 0}, 0.0f, WHITE);
            }

            // Draw difficulty labels - centered under each texture
            const char *easyText = "Level 1";
            const char *mediumText = "Level 2";
            const char *hardText = "Level 3";
            Vector2 easySize = MeasureTextEx(*pixelFont, easyText, 14, 1);
            Vector2 mediumSize = MeasureTextEx(*pixelFont, mediumText, 14, 1);
            Vector2 hardSize = MeasureTextEx(*pixelFont, hardText, 14, 1);

            DrawTextEx(*pixelFont, easyText,
                       Vector2{(float)GetScreenWidth() / 2 - 450 + 75 - easySize.x / 2, (float)GetScreenHeight() / 2 + 90},
                       14, 1, WHITE);
            DrawTextEx(*pixelFont, mediumText,
                       Vector2{(float)GetScreenWidth() / 2 - 350 + 75 - mediumSize.x / 2, (float)GetScreenHeight() / 2 + 90},
                       14, 1, WHITE);
            DrawTextEx(*pixelFont, hardText,
                       Vector2{(float)GetScreenWidth() / 2 - 250 + 75 - hardSize.x / 2, (float)GetScreenHeight() / 2 + 90},
                       14, 1, WHITE);
        }
        else if (selectedMap == MapType::MAP_2)
        {
            // Draw level textures as buttons using constructor sizes - aligned to bottom
            Rectangle easyRect = {(float)GetScreenWidth() / 2 - 150 + 25, (float)GetScreenHeight() / 2 + 30 + (60-20), 30, 20};
            Rectangle mediumRect = {(float)GetScreenWidth() / 2 - 50 + 25, (float)GetScreenHeight() / 2 + 30 + (60-40), 60, 40};
            Rectangle hardRect = {(float)GetScreenWidth() / 2 + 50 + 25, (float)GetScreenHeight() / 2 + 30, 100, 60};

            // Draw selection highlight if this difficulty is selected
            if (difficultySelected)
            {
                Rectangle highlightRect = {0, 0, 100, 60}; // Fixed size for all difficulty highlights
                switch (selectedDifficulty)
                {
                case DifficultyLevel::EASY:
                    highlightRect.x = (float)GetScreenWidth() / 2 - 150 + 25;
                    highlightRect.y = (float)GetScreenHeight() / 2 + 30;
                    break;
                case DifficultyLevel::MEDIUM:
                    highlightRect.x = (float)GetScreenWidth() / 2 - 50 + 25;
                    highlightRect.y = (float)GetScreenHeight() / 2 + 30;
                    break;
                case DifficultyLevel::HARD:
                    highlightRect.x = (float)GetScreenWidth() / 2 + 50 + 25;
                    highlightRect.y = (float)GetScreenHeight() / 2 + 30;
                    break;
                }
                DrawRectangleLines(highlightRect.x - 3, highlightRect.y - 3, highlightRect.width + 6, highlightRect.height + 6, ORANGE);
                DrawRectangleLines(highlightRect.x - 1, highlightRect.y - 1, highlightRect.width + 2, highlightRect.height + 2, ORANGE);
            }

            DrawTexturePro(ResrcManager::GetInstance().getTexture("LEVEL 1"),
                           Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("LEVEL 1").width,
                                     (float)ResrcManager::GetInstance().getTexture("LEVEL 1").height},
                           easyRect, Vector2{0, 0}, 0.0f, WHITE);
            DrawTexturePro(ResrcManager::GetInstance().getTexture("LEVEL 2"),
                           Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("LEVEL 2").width,
                                     (float)ResrcManager::GetInstance().getTexture("LEVEL 2").height},
                           mediumRect, Vector2{0, 0}, 0.0f, WHITE);
            DrawTexturePro(ResrcManager::GetInstance().getTexture("LEVEL 3"),
                           Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("LEVEL 3").width,
                                     (float)ResrcManager::GetInstance().getTexture("LEVEL 3").height},
                           hardRect, Vector2{0, 0}, 0.0f, WHITE);

            const char *easyText = "Level 1";
            const char *mediumText = "Level 2";
            const char *hardText = "Level 3";
            Vector2 easySize = MeasureTextEx(*pixelFont, easyText, 14, 1);
            Vector2 mediumSize = MeasureTextEx(*pixelFont, mediumText, 14, 1);
            Vector2 hardSize = MeasureTextEx(*pixelFont, hardText, 14, 1);

            DrawTextEx(*pixelFont, easyText,
                       Vector2{(float)GetScreenWidth() / 2 - 150 + 75 - easySize.x / 2, (float)GetScreenHeight() / 2 + 90},
                       14, 1, WHITE);
            DrawTextEx(*pixelFont, mediumText,
                       Vector2{(float)GetScreenWidth() / 2 - 50 + 75 - mediumSize.x / 2, (float)GetScreenHeight() / 2 + 90},
                       14, 1, WHITE);
            DrawTextEx(*pixelFont, hardText,
                       Vector2{(float)GetScreenWidth() / 2 + 50 + 75 - hardSize.x / 2, (float)GetScreenHeight() / 2 + 90},
                       14, 1, WHITE);
        }
        else if (selectedMap == MapType::MAP_3)
        {
            // Draw level textures as buttons using constructor sizes - aligned to bottom
            Rectangle easyRect = {(float)GetScreenWidth() / 2 + 150 + 25, (float)GetScreenHeight() / 2 + 30 + (60-20), 30, 20};
            Rectangle mediumRect = {(float)GetScreenWidth() / 2 + 250 + 25, (float)GetScreenHeight() / 2 + 30 + (60-40), 60, 40};
            Rectangle hardRect = {(float)GetScreenWidth() / 2 + 350 + 25, (float)GetScreenHeight() / 2 + 30, 100, 60};

            // Draw selection highlight if this difficulty is selected
            if (difficultySelected)
            {
                Rectangle highlightRect = {0, 0, 100, 60}; // Fixed size for all difficulty highlights
                switch (selectedDifficulty)
                {
                case DifficultyLevel::EASY:
                    highlightRect.x = (float)GetScreenWidth() / 2 + 150 + 25;
                    highlightRect.y = (float)GetScreenHeight() / 2 + 30;
                    break;
                case DifficultyLevel::MEDIUM:
                    highlightRect.x = (float)GetScreenWidth() / 2 + 250 + 25;
                    highlightRect.y = (float)GetScreenHeight() / 2 + 30;
                    break;
                case DifficultyLevel::HARD:
                    highlightRect.x = (float)GetScreenWidth() / 2 + 350 + 25;
                    highlightRect.y = (float)GetScreenHeight() / 2 + 30;
                    break;
                }
                DrawRectangleLines(highlightRect.x - 3, highlightRect.y - 3, highlightRect.width + 6, highlightRect.height + 6, ORANGE);
                DrawRectangleLines(highlightRect.x - 1, highlightRect.y - 1, highlightRect.width + 2, highlightRect.height + 2, ORANGE);
            }

            DrawTexturePro(ResrcManager::GetInstance().getTexture("LEVEL 1"),
                           Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("LEVEL 1").width,
                                     (float)ResrcManager::GetInstance().getTexture("LEVEL 1").height},
                           easyRect, Vector2{0, 0}, 0.0f, WHITE);
            DrawTexturePro(ResrcManager::GetInstance().getTexture("LEVEL 2"),
                           Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("LEVEL 2").width,
                                     (float)ResrcManager::GetInstance().getTexture("LEVEL 2").height},
                           mediumRect, Vector2{0, 0}, 0.0f, WHITE);
            DrawTexturePro(ResrcManager::GetInstance().getTexture("LEVEL 3"),
                           Rectangle{0, 0, (float)ResrcManager::GetInstance().getTexture("LEVEL 3").width,
                                     (float)ResrcManager::GetInstance().getTexture("LEVEL 3").height},
                           hardRect, Vector2{0, 0}, 0.0f, WHITE);

            const char *easyText = "Level 1";
            const char *mediumText = "Level 2";
            const char *hardText = "Level 3";
            Vector2 easySize = MeasureTextEx(*pixelFont, easyText, 14, 1);
            Vector2 mediumSize = MeasureTextEx(*pixelFont, mediumText, 14, 1);
            Vector2 hardSize = MeasureTextEx(*pixelFont, hardText, 14, 1);

            DrawTextEx(*pixelFont, easyText,
                       Vector2{(float)GetScreenWidth() / 2 + 150 + 75 - easySize.x / 2, (float)GetScreenHeight() / 2 + 90},
                       14, 1, WHITE);
            DrawTextEx(*pixelFont, mediumText,
                       Vector2{(float)GetScreenWidth() / 2 + 250 + 75 - mediumSize.x / 2, (float)GetScreenHeight() / 2 + 90},
                       14, 1, WHITE);
            DrawTextEx(*pixelFont, hardText,
                       Vector2{(float)GetScreenWidth() / 2 + 350 + 75 - hardSize.x / 2, (float)GetScreenHeight() / 2 + 90},
                       14, 1, WHITE);
        }

        // Note: Difficulty selection highlight is now drawn directly within each map section above
    }

    // Draw control buttons
    if (CanStartGame())
    {
        startButton.Draw();
    }

    backButton.Draw();

    // Draw instructions
    if (!mapSelected)
    {
        const char *selectMapText = "Please select a map first";
        Vector2 selectMapSize = MeasureTextEx(*pixelFont, selectMapText, 18, 1);
        DrawTextEx(*pixelFont, selectMapText,
                   Vector2{(float)GetScreenWidth() / 2 - selectMapSize.x / 2, (float)GetScreenHeight() - 100},
                   18, 1, RED);
    }
    else if (!difficultySelected)
    {
        const char *selectDifficultyText = "Please select a difficulty level";
        Vector2 selectDifficultySize = MeasureTextEx(*pixelFont, selectDifficultyText, 18, 1);
        DrawTextEx(*pixelFont, selectDifficultyText,
                   Vector2{(float)GetScreenWidth() / 2 - selectDifficultySize.x / 2, (float)GetScreenHeight() - 100},
                   18, 1, RED);
    }
    else
    {
        const char *startGameText = "Press START to begin the game!";
        Vector2 startGameSize = MeasureTextEx(*pixelFont, startGameText, 18, 1);
        DrawTextEx(*pixelFont, startGameText,
                   Vector2{(float)GetScreenWidth() / 2 - startGameSize.x / 2, (float)GetScreenHeight() - 100},
                   18, 1, GREEN);

        // Display speed information
        float baseSpeed = 1.0f, levelSpeed = 1.0f;
        switch (selectedDifficulty)
        {
        case DifficultyLevel::EASY:
            baseSpeed = 0.6f;
            break;
        case DifficultyLevel::MEDIUM:
            baseSpeed = 1.0f;
            break;
        case DifficultyLevel::HARD:
            baseSpeed = 1.8f;
            break;
        }
        switch (selectedMap)
        {
        case MapType::MAP_1:
            levelSpeed = 1.0f;
            break;
        case MapType::MAP_2:
            levelSpeed = 2.0f;
            break;
        case MapType::MAP_3:
            levelSpeed = 3.5f;
            break;
        }
        float totalSpeed = baseSpeed * levelSpeed;

        char speedInfo[100];
        sprintf(speedInfo, "Enemy Speed: %.1fx (Difficulty: %.1fx, Level: %.1fx)", totalSpeed, baseSpeed, levelSpeed);
        Vector2 speedInfoSize = MeasureTextEx(*pixelFont, speedInfo, 14, 1);
        DrawTextEx(*pixelFont, speedInfo,
                   Vector2{(float)GetScreenWidth() / 2 - speedInfoSize.x / 2, (float)GetScreenHeight() - 70},
                   14, 1, YELLOW);
    }
}
