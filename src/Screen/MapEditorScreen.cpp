#include "../inc/Screen/MapEditorScreen.h"
#include "../inc/Screen/MenuScreen.h"
#include "../inc/Screen/GameScreen.h"
#include <cstring>
#include <iostream>
#include <filesystem>

MapEditorScreen::MapEditorScreen(ScreenController *screenController)
    : Screen(screenController),
      currentMap(nullptr),
      // currentMode(EditorMode::TILE_MODE),
      // selectedTileId(105),
      // showSaveDialog(false),
      // showLoadDialog(false),
      backToMenuButton2(Vector2{1300, 830}, Vector2{122, 50}),
      backToMenuButton(Vector2{30, 30}, Vector2{122, 50}),
      newMapButton(Vector2{200, 30}, Vector2{198, 50}),
      loadMapButton(Vector2{450, 30}, Vector2{211, 50}),
      saveMapButton(Vector2{1450, 830}, Vector2{130, 50}),
      currentMode(EditorMode::TILE_MODE),
      selectedTileId(105),
      showLoadDialog(false),
      showSaveDialog(false),
      selectedMapIndex(-1),
      mapListScrollY(0),
      tilesetScrollY(0)
{
    camera.offset = Vector2{(GetScreenWidth() - TILESET_PANEL_WIDTH) / 2, (float)GetScreenHeight() / 2};
    camera.target = Vector2{0, 0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    currentMap = new Map();
    LoadAvailableMaps();
    InitializeTilesets();
    strcpy(mapNameBuffer, "new_map");
    backgroundTexture = &ResrcManager::GetInstance().getTexture("MENU_BACKGROUND");
    backToMenuButton.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
    newMapButton.SetTexture(ResrcManager::GetInstance().getTexture("NEW_MAP_BUTTON"));
    loadMapButton.SetTexture(ResrcManager::GetInstance().getTexture("LOAD_MAP_BUTTON"));
    backToMenuButton2.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
}

MapEditorScreen::~MapEditorScreen() = default;

void MapEditorScreen::InitializeTilesets()
{
    // Clear existing data
    availableTiles.clear();
    availableBlocks.clear();
    availableEnemies.clear();
    availableItems.clear();

    // TILES (1-104): Basic terrain tiles
    for (int i = 1; i <= 104; i++)
    {
        TilesetTile tile;
        tile.tileId = i;
        tile.sourceRect = {0, 0, 32, 32};

        // Try to get texture from ResrcManager
        std::string textureName = "tile_" + std::to_string(i);
        try
        {
            tile.texture = ResrcManager::GetInstance().getTexture(textureName);
        }
        catch (...)
        {
            tile.texture = {0}; // Empty texture if not found
        }

        availableTiles.push_back(tile);
    }

    // BLOCKS (104-120): Interactive blocks
    std::vector<int> blockIds = {105, 106, 107, 111, 114, 115, 116, 117, 119, 120};
    for (int blockId : blockIds)
    {
        TilesetTile block;
        block.tileId = blockId;
        block.sourceRect = {0, 0, 32, 32};
        switch (blockId)
        {
        case 105:
            block.texture = ResrcManager::GetInstance().getTexture("BLOCK_CLOUD");
            break;
        case 106:
            block.texture = ResrcManager::GetInstance().getTexture("BLOCK_EYE_OPEN_0");
            break;
        case 107:
            block.texture = ResrcManager::GetInstance().getTexture("BLOCK_EYE_CLOSED");
            break;
        case 111:
            block.texture = ResrcManager::GetInstance().getTexture("BLOCK_GLASS");
            break;
        case 114:
            block.texture = ResrcManager::GetInstance().getTexture("");
            break;
        case 115:
            block.texture = ResrcManager::GetInstance().getTexture("BLOCK_QUESTION_0");
            break;
        case 116:
            block.texture = ResrcManager::GetInstance().getTexture("BLOCK_QUESTION_1");
            break;
        case 117:
            block.texture = ResrcManager::GetInstance().getTexture("BLOCK_QUESTION_2");
            break;
        case 119:
            block.texture = ResrcManager::GetInstance().getTexture("");
            break;
        case 120:
            block.texture = ResrcManager::GetInstance().getTexture("BLOCK_WOOD");
            break;
        default:
            break;
        }
        availableBlocks.push_back(block);
    }

    // ENEMIES (121-136): All enemy types
    std::vector<int> enemyIds = {121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136};
    for (int enemyId : enemyIds)
    {
        TilesetTile enemy;
        enemy.tileId = enemyId;
        switch (enemyId)
        {
        case 121:
            enemy.sourceRect = {0, 0, 32, 54};
            enemy.texture = ResrcManager::GetInstance().getTexture("BlueKoopaTroopa_0_RIGHT");
            break;
        case 122:
            enemy.sourceRect = {0, 0, 24, 30};
            enemy.texture = ResrcManager::GetInstance().getTexture("Bob-omb_Right_0");
            break;
        case 123:
            enemy.sourceRect = {0, 0, 32, 28};
            enemy.texture = ResrcManager::GetInstance().getTexture("BulletBill");
            break;
        case 124:
            enemy.sourceRect = {0, 0, 32, 32};
            enemy.texture = ResrcManager::GetInstance().getTexture("BuzzyBeetle_0_Right");
            break;
        case 125:
            enemy.sourceRect = {0, 0, 66, 50};
            enemy.texture = ResrcManager::GetInstance().getTexture("FlyingGoomba_Flap_Center");
            break;
        case 126:
            enemy.sourceRect = {0, 0, 32, 32};
            enemy.texture = ResrcManager::GetInstance().getTexture("GOOMBA_0_RIGHT");
            break;
        case 127:
            enemy.sourceRect = {0, 0, 32, 54};
            enemy.texture = ResrcManager::GetInstance().getTexture("GreenKoopaTroopa_0_RIGHT");
            break;
        case 128:
            enemy.sourceRect = {0, 0, 32, 66};
            enemy.texture = ResrcManager::GetInstance().getTexture("PiranhaPlant_MouthOpen");
            break;
        case 129:
            enemy.sourceRect = {0, 0, 32, 54};
            enemy.texture = ResrcManager::GetInstance().getTexture("RedKoopaTroopa_0_RIGHT");
            break;
        case 130:
            enemy.sourceRect = {0, 0, 32, 32};
            enemy.texture = ResrcManager::GetInstance().getTexture("");
            break;
        case 131:
            enemy.sourceRect = {0, 0, 32, 32};
            enemy.texture = ResrcManager::GetInstance().getTexture("");
            break;
        case 132:
            enemy.sourceRect = {0, 0, 32, 32};
            enemy.texture = ResrcManager::GetInstance().getTexture("");
            break;
        case 133:
            enemy.sourceRect = {0, 0, 32, 54};
            enemy.texture = ResrcManager::GetInstance().getTexture("");
            break;
        case 134:
            enemy.sourceRect = {0, 0, 40, 64};
            enemy.texture = ResrcManager::GetInstance().getTexture("");
            break;
        case 135:
            enemy.sourceRect = {0, 0, 40, 64};
            enemy.texture = ResrcManager::GetInstance().getTexture("REX_0_RIGHT");
            break;
        case 136:
            enemy.sourceRect = {0, 0, 32, 54};
            enemy.texture = ResrcManager::GetInstance().getTexture("YellowKoopaTroopa_0_RIGHT");
            break;
        default:
            break;
        }
        availableEnemies.push_back(enemy);
    }

    // ITEMS (137-143): Power-ups and collectibles
    std::vector<int> itemIds = {137, 138, 139, 140, 141, 142, 143};
    for (int itemId : itemIds)
    {
        TilesetTile item;
        item.tileId = itemId;
        switch (itemId)
        {
        case 137:
            item.sourceRect = {0, 0, 32, 32};
            item.texture = ResrcManager::GetInstance().getTexture("1UpMushroom");
            break;
        case 138:
            item.sourceRect = {0, 0, 30, 32};
            item.texture = ResrcManager::GetInstance().getTexture("3UpMoon");
            break;
        case 139:
            item.sourceRect = {0, 0, 24, 32};
            item.texture = ResrcManager::GetInstance().getTexture("COIN_2");
            break;
        case 140:
            item.sourceRect = {0, 0, 32, 32};
            item.texture = ResrcManager::GetInstance().getTexture("FIRE FLOWER_0");
            break;
        case 141:
            item.sourceRect = {0, 0, 32, 32};
            item.texture = ResrcManager::GetInstance().getTexture("Mushroom");
            break;
        case 142:
            item.sourceRect = {0, 0, 30, 32};
            item.texture = ResrcManager::GetInstance().getTexture("star");
            break;
        case 143:
            item.sourceRect = {0, 0, 32, 50};
            item.texture = ResrcManager::GetInstance().getTexture("YoshiCoin_0");
            break;
        }

        availableItems.push_back(item);
    }

    std::cout << "Initialized tilesets: "
              << availableTiles.size() << " tiles, "
              << availableBlocks.size() << " blocks, "
              << availableEnemies.size() << " enemies, "
              << availableItems.size() << " items" << std::endl;
}

void MapEditorScreen::Update()
{
    backToMenuButton.Update();
    newMapButton.Update();
    loadMapButton.Update();
    saveMapButton.Update();

    if (IsKeyPressed(KEY_ONE))
        currentMode = EditorMode::TILE_MODE;
    if (IsKeyPressed(KEY_TWO))
        currentMode = EditorMode::BLOCK_MODE;
    if (IsKeyPressed(KEY_THREE))
        currentMode = EditorMode::ENEMY_MODE;
    if (IsKeyPressed(KEY_FOUR))
        currentMode = EditorMode::ITEM_MODE;
    UpdateCamera();
    HandleUIEvents();
    if (editorActive)
    {
        HandleMapInput();
    }
    if (backToMenuButton.IsPressed() || backToMenuButton2.IsPressed())
    {
        screenController->ChangeScreen(new MenuScreen(screenController));
    }
    else if (newMapButton.IsPressed())
    {
        std::cout << "New Map button pressed!" << std::endl;
        // Logic to create a new map
        // currentMap->LoadMap(0); // Load the first map or create a new one
    }
    else if (loadMapButton.IsPressed())
    {
        std::cout << "Load Map button pressed!" << std::endl;
        showLoadDialog = true;
        selectedMapIndex = -1;
    }
    else if (saveMapButton.IsPressed())
    {
        std::cout << "Save Map button pressed!" << std::endl;
        SaveMap(mapNameBuffer);
        showSaveDialog = true;
        strcpy(mapNameBuffer, "new_map");
    }
};

void MapEditorScreen::DrawSaveDialog()
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

    // Dialog box
    Rectangle dialogBox = {
        (float)(GetScreenWidth() / 2 - 250),
        (float)(GetScreenHeight() / 2 - 120),
        500, 240};
    DrawRectangleRounded(dialogBox, 0.1f, 10, WHITE);
    DrawRectangleRoundedLines(dialogBox, 0.1f, 10, DARKGRAY);

    // Title
    DrawText("Save Map", (int)(dialogBox.x + 20), (int)(dialogBox.y + 20), 28, BLACK);
    DrawText("Enter map name (without .json extension):",
             (int)(dialogBox.x + 20), (int)(dialogBox.y + 60), 18, DARKGRAY);

    // Input field
    Rectangle inputField = {
        dialogBox.x + 20,
        dialogBox.y + 90,
        460, 35};
    DrawRectangleRec(inputField, LIGHTGRAY);
    DrawRectangleLines((int)inputField.x, (int)inputField.y, (int)inputField.width, (int)inputField.height, GRAY);

    // Draw text in input field
    DrawText(mapNameBuffer, (int)(inputField.x + 10), (int)(inputField.y + 8), 20, BLACK);

    // Cursor blinking effect
    static float blinkTimer = 0;
    blinkTimer += GetFrameTime();
    if (fmod(blinkTimer, 1.0f) < 0.5f)
    {
        int textWidth = MeasureText(mapNameBuffer, 20);
        DrawLine((int)(inputField.x + 10 + textWidth), (int)(inputField.y + 8),
                 (int)(inputField.x + 10 + textWidth), (int)(inputField.y + 27), BLACK);
    }

    // Buttons
    Rectangle saveBtn = {dialogBox.x + 120, dialogBox.y + 160, 100, 40};
    Rectangle cancelBtn = {dialogBox.x + 280, dialogBox.y + 160, 100, 40};

    // Save button
    bool canSave = (strlen(mapNameBuffer) > 0);
    Color saveBtnColor = canSave ? GREEN : GRAY;

    DrawRectangleRec(saveBtn, saveBtnColor);
    DrawRectangleLines((int)saveBtn.x, (int)saveBtn.y, (int)saveBtn.width, (int)saveBtn.height, DARKGRAY);
    DrawText("Save", (int)(saveBtn.x + 30), (int)(saveBtn.y + 12), 20, WHITE);

    if (canSave && CheckCollisionPointRec(GetMousePosition(), saveBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        std::string filename = std::string(mapNameBuffer);
        SaveMap(filename);
        std::cout << "Map saved as: " << filename << ".json" << std::endl;
        showSaveDialog = false;
    }

    // Cancel button
    DrawRectangleRec(cancelBtn, RED);
    DrawRectangleLines((int)cancelBtn.x, (int)cancelBtn.y, (int)cancelBtn.width, (int)cancelBtn.height, DARKGRAY);
    DrawText("Cancel", (int)(cancelBtn.x + 25), (int)(cancelBtn.y + 12), 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), cancelBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        showSaveDialog = false;
    }

    // Instructions
    DrawText("Use backspace to delete, ESC to cancel, Enter to save",
             (int)(dialogBox.x + 20), (int)(dialogBox.y + 210), 14, GRAY);
}

void MapEditorScreen::SaveMap(const std::string &mapName)
{
    std::string fullPath = "resources/maps/" + mapName + ".json";

    try
    {
        nlohmann::json mapData;

        // Basic map info
        mapData["compressionlevel"] = -1;
        mapData["height"] = 30;
        mapData["infinite"] = false;
        mapData["orientation"] = "orthogonal";
        mapData["renderorder"] = "right-down";
        mapData["tiledversion"] = "1.11.2";
        mapData["tileheight"] = 32;
        mapData["tilewidth"] = 32;
        mapData["type"] = "map";
        mapData["version"] = "1.10";
        mapData["width"] = 150;
        mapData["nextlayerid"] = 2;
        mapData["nextobjectid"] = 1;

        // Create tileset reference
        mapData["tilesets"] = nlohmann::json::array();
        mapData["tilesets"].push_back({{"firstgid", 1},
                                       {"source", mapName + ".tsx"}});

        // Create layer data array (150 x 30 = 4500 tiles)
        std::vector<int> layerData(150 * 30, 0);

        // Add tiles to layer data
        for (auto &tile : currentMap->getInteractiveTiles())
        {
            if (tile)
            {
                int x = (int)(tile->GetPos().x / 32);
                int y = (int)(tile->GetPos().y / 32);
                if (x >= 0 && x < 150 && y >= 0 && y < 30)
                {
                    layerData[y * 150 + x] = tile->GetKey();
                }
            }
        }

        // Add blocks to layer data
        for (auto &block : currentMap->getBlocks())
        {
            if (block)
            {
                int x = (int)(block->GetPos().x / 32);
                int y = (int)(block->GetPos().y / 32);
                if (x >= 0 && x < 150 && y >= 0 && y < 30)
                {
                    // Map block types to tile IDs
                    int tileId = GetBlockTileId(block);
                    if (tileId > 0)
                    {
                        layerData[y * 150 + x] = tileId;
                    }
                }
            }
        }

        // Add enemies to layer data
        for (auto &enemy : currentMap->GetEnemies())
        {
            if (enemy)
            {
                int x = (int)(enemy->GetPos().x / 32);
                int y = (int)((enemy->GetPos().y + 20) / 32); // Adjust for enemy positioning
                if (x >= 0 && x < 150 && y >= 0 && y < 30)
                {
                    int tileId = GetEnemyTileId(enemy);
                    if (tileId > 0)
                    {
                        layerData[y * 150 + x] = tileId;
                    }
                }
            }
        }

        // Add items to layer data
        for (auto &item : currentMap->GetInteractiveItems())
        {
            if (item)
            {
                int x = (int)(item->GetPos().x / 32);
                int y = (int)(item->GetPos().y / 32);
                if (x >= 0 && x < 150 && y >= 0 && y < 30)
                {
                    int tileId = GetItemTileId(item.get());
                    if (tileId > 0)
                    {
                        layerData[y * 150 + x] = tileId;
                    }
                }
            }
        }

        // Create layer
        mapData["layers"] = nlohmann::json::array();
        mapData["layers"].push_back({{"data", layerData},
                                     {"height", 30},
                                     {"id", 1},
                                     {"name", "Tile Layer 1"},
                                     {"opacity", 1},
                                     {"type", "tilelayer"},
                                     {"visible", true},
                                     {"width", 150},
                                     {"x", 0},
                                     {"y", 0}});

        // Write to file
        std::ofstream file(fullPath);
        file << mapData.dump(4); // Pretty print with 4 spaces
        file.close();

        std::cout << "Map successfully saved to: " << fullPath << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error saving map: " << e.what() << std::endl;
    }
}

// Helper functions to get tile IDs
int MapEditorScreen::GetBlockTileId(Block *block)
{
    if (dynamic_cast<QuestionBlock *>(block)){
        if(dynamic_cast<QuestionBlock *>(block)->GetGiftType() == GIFT_COIN)
            return 117; // Coin block
        if(dynamic_cast<QuestionBlock *>(block)->GetGiftType() == GIFT_FIRE_FLOWER)
            return 118; // Fire flower block
        return 116; // Default question block
    }
    else if (dynamic_cast<WoodBlock *>(block))
        return 121;
    else if (dynamic_cast<GlassBlock *>(block))
        return 112;
    else if (dynamic_cast<CloudBlock *>(block))
        return 105;
    else if (dynamic_cast<EyesClosedBlock *>(block))
        return 107; // Closed eye
    else if (dynamic_cast<EyesOpenedBlock *>(block))
        return 108; // Open eye
    // Add more mappings as needed
    return 0;
}

int MapEditorScreen::GetEnemyTileId(Enemy *enemy)
{

    if (dynamic_cast<Goomba *>(enemy))
        return 126;
    else if (dynamic_cast<GreenKoopa *>(enemy))
        return 127;
    else if (dynamic_cast<RedKoopa *>(enemy))
        return 129;
    else if (dynamic_cast<Rex *>(enemy))
        return 135;
    else if (dynamic_cast<Bob_omb *>(enemy))
        return 122;
    else if (dynamic_cast<BuzzyBeetle *>(enemy))
        return 124;
    else if (dynamic_cast<FlyingGoomba *>(enemy))
        return 125;
    else if (dynamic_cast<BulletBill *>(enemy))
        return 123;
    else if (dynamic_cast<PiranhaPlant *>(enemy))
        return 128;
    else if (dynamic_cast<YellowKoopa *>(enemy))
        return 136;
    return 0;
}

int MapEditorScreen::GetItemTileId(Item *item)
{

    if (dynamic_cast<Coin *>(item))
        return 140;
    else if (dynamic_cast<Mushroom *>(item))
        return 142;
    else if (dynamic_cast<FireFlower *>(item))
        return 141;
    else if (dynamic_cast<Star *>(item))
        return 143;
    else if (dynamic_cast<OneUpMushroom *>(item))
        return 138;
    else if (dynamic_cast<ThreeUpMoon *>(item))
        return 139;
    else if (dynamic_cast<YoshiCoin *>(item))
        return 144;

    return 0;
}

void MapEditorScreen::HandleMapInput()
{
    if (!IsMouseInMainViewport())
        return;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        PlaceObject();
    }

    // Right click to remove objec
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        RemoveObject();
    }

    // Handle tileset panel scrolling
    if (CheckCollisionPointRec(GetMousePosition(), GetTilesetPanel()))
    {
        float wheelMove = GetMouseWheelMove();
        if (wheelMove != 0)
        {
            tilesetScrollY -= wheelMove * 30;
            if (tilesetScrollY < 0)
                tilesetScrollY = 0;

            // Calculate max scroll based on tileset content
            int maxRows = 0;
            switch (currentMode)
            {
            case EditorMode::TILE_MODE:
                maxRows = (availableTiles.size() + 5) / 6;
                break;
            case EditorMode::BLOCK_MODE:
                maxRows = (availableBlocks.size() + 5) / 6;
                break;
            case EditorMode::ENEMY_MODE:
                maxRows = (availableEnemies.size() + 2) / 3;
                break;
            case EditorMode::ITEM_MODE:
                maxRows = (availableItems.size() + 5) / 6;
                break;
            }

            float maxScroll = std::max(0.0f, maxRows * 75.0f - GetTilesetPanel().height + 100);
            if (tilesetScrollY > maxScroll)
                tilesetScrollY = maxScroll;
        }
    }
}

void MapEditorScreen::PlaceObject()
{
    Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);
    Vector2 gridPos = ScreenToGrid(worldPos);

    // Convert to tile coordinates
    int tileX = (int)gridPos.x;
    int tileY = (int)gridPos.y;

    std::cout << "Placing " << selectedTileId << " at (" << tileX << ", " << tileY << ")" << std::endl;

    switch (currentMode)
    {
    case EditorMode::TILE_MODE:
    {
        // Add tile to map
        if (selectedTileId >= 0 && selectedTileId <= 104)
        {
            // Remove existing tile at this position first
            RemoveObjectAt(tileX, tileY, currentMode);

            // Add new tile
            Vector2 worldPosition = GridToWorld(gridPos);
            Tile *newTile = new Tile(worldPosition, selectedTileId, selectedTileId);
            currentMap->getInteractiveTiles().push_back(newTile);

            std::cout << "Placed tile " << selectedTileId << " at world pos ("
                      << worldPosition.x << ", " << worldPosition.y << ")" << std::endl;
        }
        break;
    }

    case EditorMode::BLOCK_MODE:
    {
        // Add block to map
        if (selectedTileId >= 105 && selectedTileId <= 120)
        {
            RemoveObjectAt(tileX, tileY, currentMode);

            Vector2 worldPosition = GridToWorld(gridPos);
            Block *newBlock = CreateBlockByType(selectedTileId, worldPosition);
            if (newBlock)
            {
                currentMap->getBlocks().push_back(newBlock);
                std::cout << "Placed block " << selectedTileId << " at world pos ("
                          << worldPosition.x << ", " << worldPosition.y << ")" << std::endl;
            }
        }
        break;
    }

    case EditorMode::ENEMY_MODE:
    {
        // Add enemy to map
        if (selectedTileId >= 121 && selectedTileId <= 136)
        {
            RemoveObjectAt(tileX, tileY, currentMode);

            Vector2 worldPosition = GridToWorld(gridPos);
            worldPosition.y -= 20; // Adjust for enemy positioning

            Enemy *newEnemy = CreateEnemyByType(selectedTileId, worldPosition);
            if (newEnemy)
            {
                currentMap->GetEnemies().push_back(newEnemy);
                std::cout << "Placed enemy " << selectedTileId << " at world pos ("
                          << worldPosition.x << ", " << worldPosition.y << ")" << std::endl;
            }
        }
        break;
    }

    case EditorMode::ITEM_MODE:
    {
        // Add item to map
        if (selectedTileId >= 137 && selectedTileId <= 143)
        {
            RemoveObjectAt(tileX, tileY, currentMode);

            Vector2 worldPosition = GridToWorld(gridPos);
            std::shared_ptr<Item> newItem = CreateItemByType(selectedTileId, worldPosition);
            if (newItem)
            {
                currentMap->GetInteractiveItems().push_back(newItem);
                std::cout << "Placed item " << selectedTileId << " at world pos ("
                          << worldPosition.x << ", " << worldPosition.y << ")" << std::endl;
            }
        }
        break;
    }
    }
}

void MapEditorScreen::RemoveObject()
{
    Vector2 worldPos = GetScreenToWorld2D(GetMousePosition(), camera);
    Vector2 gridPos = ScreenToGrid(worldPos);

    int tileX = (int)gridPos.x;
    int tileY = (int)gridPos.y;

    RemoveObjectAt(tileX, tileY, currentMode);
}

void MapEditorScreen::RemoveObjectAt(int tileX, int tileY, EditorMode mode)
{
    Vector2 worldPos = GridToWorld(Vector2{(float)tileX, (float)tileY});
    const float TOLERANCE = 16.0f; // Half tile size

    switch (mode)
    {
    case EditorMode::TILE_MODE:
    {
        auto &tiles = currentMap->getInteractiveTiles();
        for (auto it = tiles.begin(); it != tiles.end(); ++it)
        {
            if (*it && fabs((*it)->GetPos().x - worldPos.x) < TOLERANCE &&
                fabs((*it)->GetPos().y - worldPos.y) < TOLERANCE)
            {
                delete *it;
                tiles.erase(it);
                std::cout << "Removed tile at (" << tileX << ", " << tileY << ")" << std::endl;
                break;
            }
        }
        break;
    }

    case EditorMode::BLOCK_MODE:
    {
        auto &blocks = currentMap->getBlocks();
        for (auto it = blocks.begin(); it != blocks.end(); ++it)
        {
            if (*it && fabs((*it)->GetPos().x - worldPos.x) < TOLERANCE &&
                fabs((*it)->GetPos().y - worldPos.y) < TOLERANCE)
            {
                delete *it;
                blocks.erase(it);
                std::cout << "Removed block at (" << tileX << ", " << tileY << ")" << std::endl;
                break;
            }
        }
        break;
    }

    case EditorMode::ENEMY_MODE:
    {
        auto &enemies = currentMap->GetEnemies();
        for (auto it = enemies.begin(); it != enemies.end(); ++it)
        {
            if (*it && fabs((*it)->GetPos().x - worldPos.x) < TOLERANCE &&
                fabs((*it)->GetPos().y - worldPos.y ) < TOLERANCE)
            {
                delete *it;
                enemies.erase(it);
                std::cout << "Removed enemy at (" << tileX << ", " << tileY << ")" << std::endl;
                break;
            }
        }
        break;
    }

    case EditorMode::ITEM_MODE:
    {
        auto &items = currentMap->GetInteractiveItems();
        for (auto it = items.begin(); it != items.end(); ++it)
        {
            if (*it && fabs((*it)->GetPos().x - worldPos.x) < TOLERANCE &&
                fabs((*it)->GetPos().y - worldPos.y) < TOLERANCE)
            {
                items.erase(it);
                std::cout << "Removed item at (" << tileX << ", " << tileY << ")" << std::endl;
                break;
            }
        }
        break;
    }
    }
}

// Helper functions
Vector2 MapEditorScreen::ScreenToGrid(Vector2 worldPos) const
{
    return Vector2{
        (float)floor(worldPos.x / GRID_SIZE),
        (float)floor(worldPos.y / GRID_SIZE)};
}

Vector2 MapEditorScreen::GridToWorld(Vector2 gridPos) const
{
    return Vector2{
        gridPos.x * GRID_SIZE,
        gridPos.y * GRID_SIZE};
}

bool MapEditorScreen::IsMouseInMainViewport() const
{
    Rectangle viewport = GetMainViewport();
    return CheckCollisionPointRec(GetMousePosition(), viewport);
}

// Factory functions để tạo objects
Block* MapEditorScreen::CreateBlockByType(int tileId, Vector2 position)
{   
    switch (tileId)
    {
    case 105:
        return new CloudBlock(position, {32, 32}, WHITE);
        break;
    case 106:
        return new EyesOpenedBlock(position, {32, 32}, WHITE);
        break;
    case 107:
        return new EyesClosedBlock(position, {32, 32}, WHITE);
        break;
    case 111:
        return new GlassBlock(position, {32, 32}, WHITE);
        break;
    case 115:
        return new QuestionBlock(position, {32, 32}, WHITE, GIFT_NONE);
        break;
    case 116:
        return  new QuestionBlock(position, {32, 32}, WHITE, GIFT_COIN);
        break;
    case 117:
        return new QuestionBlock(position, {32, 32}, WHITE, GIFT_FIRE_FLOWER);
        break;
    case 120:
        return new WoodBlock(position, {32, 32}, WHITE);
        break;
    default:
        return nullptr;
    }
}

Enemy *MapEditorScreen::CreateEnemyByType(int tileId, Vector2 position)
{
    switch (tileId)
    {
    case 122:
        return new Bob_omb(position);
    case 123:
        return new BulletBill(position);
    case 124:
        return new BuzzyBeetle(position);
    case 125:
        return new FlyingGoomba(position);
    case 126:
        return new Goomba(position);
    case 127:
        return new GreenKoopa(position);
    case 129:
        return new RedKoopa(position);
    case 135:
        return new Rex(position);
    case 128:
        return new PiranhaPlant(position);
    case 136 :
        return new YellowKoopa(position);
    default:
        return nullptr;
    }
}

std::shared_ptr<Item> MapEditorScreen::CreateItemByType(int tileId, Vector2 position)
{
    switch (tileId)
    {
    case 137:
        return std::make_shared<OneUpMushroom>(position);
    case 138:
        return std::make_shared<ThreeUpMoon>(position);
    case 139:
        return std::make_shared<Coin>(position);
    case 140:
        return std::make_shared<FireFlower>(position);
    case 141:
        return std::make_shared<Mushroom>(position);
    case 142:
        return std::make_shared<Star>(position);
    case 143:
        return std::make_shared<YoshiCoin>(position);
    default:
        return nullptr;
    }
}

void MapEditorScreen::DrawCursorPreview()
{
    // Get mouse position in world coordinates
    Vector2 worldMousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    // Snap to grid
    int gridX = (int)(worldMousePos.x / GRID_SIZE);
    int gridY = (int)(worldMousePos.y / GRID_SIZE);

    Rectangle cursorRect = {
        (float)gridX * GRID_SIZE,
        (float)gridY * GRID_SIZE,
        (float)GRID_SIZE,
        (float)GRID_SIZE};

    // Draw preview rectangle
    DrawRectangleLinesEx(cursorRect, 2, RED);

    // Draw semi-transparent preview of selected tile
    Color previewColor = WHITE;
    switch (currentMode)
    {
    case EditorMode::TILE_MODE:
        previewColor = BLUE;
        break;
    case EditorMode::BLOCK_MODE:
        previewColor = ORANGE;
        break;
    case EditorMode::ENEMY_MODE:
        previewColor = RED;
        break;
    case EditorMode::ITEM_MODE:
        previewColor = GOLD;
        break;
    }

    DrawRectangleRec(cursorRect, Fade(previewColor, 0.3f));

    // Show tile ID
    DrawText(std::to_string(selectedTileId).c_str(),
             cursorRect.x + 5, cursorRect.y + 5, 12, BLACK);
}

void MapEditorScreen::DrawToolbar()
{
    Rectangle toolbar = {0, 0, (float)GetScreenWidth(), TOOLBAR_HEIGHT};

    // Toolbar background
    DrawRectangleRec(toolbar, Fade(BLACK, 0.8f));
    DrawRectangleLinesEx(toolbar, 2, WHITE);

    // Current mode indicator
    const char *modeNames[] = {"TILE", "BLOCK", "ENEMY", "ITEM"};
    std::string modeText = "Mode: " + std::string(modeNames[(int)currentMode]);
    DrawText(modeText.c_str(), 10, 15, 20, YELLOW);

    // Selected tile ID indicator
    std::string selectedText = "Selected ID: " + std::to_string(selectedTileId);
    DrawText(selectedText.c_str(), 200, 15, 16, WHITE);

    // Instructions
    DrawText("Controls: WASD - Move Camera | Mouse Wheel - Zoom | 1-4 - Switch Modes | LMB - Place | RMB - Remove",
             400, 15, 14, LIGHTGRAY);

    // Mode switch shortcuts
    DrawText("[1] Tiles  [2] Blocks  [3] Enemies  [4] Items",
             400, 35, 12, GRAY);
}

void MapEditorScreen::Draw()
{
    ClearBackground(GRAY);
    if (!editorActive)
    {
        DrawTexturePro(*backgroundTexture, Rectangle{0, 0, (float)backgroundTexture->width, (float)backgroundTexture->height},
                       Rectangle{0, 0, 1600, 900}, Vector2{0, 0}, 0.0f, WHITE);
        backToMenuButton.Draw();
        newMapButton.Draw();
        loadMapButton.Draw();
        if (showLoadDialog)
            DrawLoadDialog();
    }
    if (editorActive)
    {
        DrawMainEditor();
        DrawTilesetPanel();
        DrawToolbar();
        backToMenuButton2.Draw();
        saveMapButton.Draw();
    }
}

void MapEditorScreen::DrawMainEditor()
{
    Rectangle mainViewport = GetMainViewport();

    // Clip to main editor area
    BeginScissorMode(mainViewport.x, mainViewport.y, mainViewport.width, mainViewport.height);

    BeginMode2D(camera);

    // Draw grid
    DrawMapGrid();

    // Draw map content
    if (currentMap)
    {
        currentMap->Draw();
    }

    // Draw cursor preview
    DrawCursorPreview();

    EndMode2D();

    EndScissorMode();

    // Draw border around main area
    DrawRectangleLinesEx(mainViewport, 2, WHITE);
}

void MapEditorScreen::DrawMapGrid()
{
    Color gridColor = Fade(WHITE, 0.3f);

    // Vertical lines
    for (int x = 0; x <= currentMap->getWidth(); x += GRID_SIZE)
    {
        DrawLine(x, 0, x, currentMap->getHeight(), gridColor);
    }

    // Horizontal lines
    for (int y = 0; y <= currentMap->getHeight(); y += GRID_SIZE)
    {
        DrawLine(0, y, currentMap->getWidth(), y, gridColor);
    }

    // Highlight origin
    DrawLine(0, 0, 50, 0, RED);
    DrawLine(0, 0, 0, 50, RED);
}

void MapEditorScreen::DrawTilesetPanel()
{
    Rectangle panel = GetTilesetPanel();

    // Panel background
    DrawRectangleRec(panel, LIGHTGRAY);
    DrawRectangleLinesEx(panel, 2, DARKGRAY);

    // Title
    DrawText("Tilesets", panel.x + 10, panel.y + 10, 20, BLACK);

    // Mode tabs
    const char *modeNames[] = {"Tiles", "Blocks", "Enemies", "Items"};
    float tabWidth = panel.width / 4;

    for (int i = 0; i < 4; i++)
    {
        Rectangle tabRect = {
            panel.x + i * tabWidth,
            panel.y + 40,
            tabWidth,
            30};

        Color tabColor = (i == (int)currentMode) ? BLUE : GRAY;
        DrawRectangleRec(tabRect, tabColor);
        DrawRectangleLinesEx(tabRect, 1, DARKGRAY);

        // Center text in tab
        int textWidth = MeasureText(modeNames[i], 12);
        DrawText(modeNames[i],
                 tabRect.x + (tabWidth - textWidth) / 2,
                 tabRect.y + 9, 12, WHITE);

        // Handle tab click
        if (CheckCollisionPointRec(GetMousePosition(), tabRect) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            currentMode = (EditorMode)i;
        }
    }

    // Tileset content area
    Rectangle contentArea = {
        panel.x + 10,
        panel.y + 80,
        panel.width - 20,
        panel.height - 90};

    BeginScissorMode(contentArea.x, contentArea.y, contentArea.width, contentArea.height);

    // Draw tileset based on current mode
    DrawTilesetContent(contentArea);

    EndScissorMode();
}

void MapEditorScreen::DrawTilesetContent(Rectangle area)
{
    int TILES_PER_ROW = 6;
    int TILE_SIZE = 32;
    const int SPACING = 5;

    std::vector<TilesetTile> *currentTileset = nullptr;

    switch (currentMode)
    {
    case EditorMode::TILE_MODE:
        currentTileset = &availableTiles;
        break;
    case EditorMode::BLOCK_MODE:
        currentTileset = &availableBlocks;
        break;
    case EditorMode::ENEMY_MODE:
        currentTileset = &availableEnemies;
        break;
    case EditorMode::ITEM_MODE:
        currentTileset = &availableItems;
        break;
    }
    if (EditorMode::ENEMY_MODE == currentMode)
    {
        TILES_PER_ROW = 3; // Enemies have larger sprites
        TILE_SIZE = 70;    // Adjust size for enemies
    }
    if (!currentTileset)
        return;

    for (int i = 0; i < currentTileset->size(); i++)
    {
        int col = i % TILES_PER_ROW;
        int row = i / TILES_PER_ROW;

        Rectangle tileRect = {
            area.x + col * (TILE_SIZE + SPACING),
            area.y + row * (TILE_SIZE + SPACING) - tilesetScrollY,
            (float)TILE_SIZE,
            (float)TILE_SIZE};

        // Skip if not visible
        if (tileRect.y + TILE_SIZE < area.y || tileRect.y > area.y + area.height)
        {
            continue;
        }

        TilesetTile &tile = (*currentTileset)[i];

        // Draw tile texture or placeholder
        if (tile.texture.id != 0)
        {
            DrawTextureRec(tile.texture, tile.sourceRect,
                           Vector2{tileRect.x, tileRect.y}, WHITE);
        }
        else
        {
            DrawRectangleRec(tileRect, GRAY);
            DrawText(std::to_string(tile.tileId).c_str(),
                     tileRect.x + 5, tileRect.y + 10, 10, BLACK);
        }

        // Highlight selected
        if (selectedTileId == tile.tileId)
        {
            DrawRectangleLinesEx(tileRect, 3, YELLOW);
        }

        // Handle click
        if (CheckCollisionPointRec(GetMousePosition(), tileRect) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selectedTileId = tile.tileId;
        }
    }
}

void MapEditorScreen::LoadMap(const std::string &mapName)
{
    if (currentMap)
    {
        delete currentMap;
    }

    currentMap = new Map();
    std::string fullPath = "resources/maps/" + mapName + ".json";

    try
    {
        // Giả sử bạn có Map::LoadFromFile
        currentMap->LoadFromJsonFile(fullPath);

        std::cout << "Loaded map: " << mapName << std::endl;
        camera.target = Vector2{0, 0};
        camera.zoom = 1.0f;

        editorActive = true;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error loading map " << mapName << ": " << e.what() << std::endl;
    }
}

Rectangle MapEditorScreen::GetMainViewport() const
{
    return Rectangle{
        0,
        TOOLBAR_HEIGHT,
        GetScreenWidth() - TILESET_PANEL_WIDTH,
        GetScreenHeight() - TOOLBAR_HEIGHT};
}

Rectangle MapEditorScreen::GetTilesetPanel() const
{
    return Rectangle{
        GetScreenWidth() - TILESET_PANEL_WIDTH,
        TOOLBAR_HEIGHT,
        TILESET_PANEL_WIDTH,
        GetScreenHeight() - TOOLBAR_HEIGHT};
}

void MapEditorScreen::UpdateCamera()
{
    // Camera controls (WASD)
    const float CAMERA_SPEED = 300.0f;

    if (IsKeyDown(KEY_W))
        camera.target.y -= CAMERA_SPEED * GetFrameTime();
    if (IsKeyDown(KEY_S))
        camera.target.y += CAMERA_SPEED * GetFrameTime();
    if (IsKeyDown(KEY_A))
        camera.target.x -= CAMERA_SPEED * GetFrameTime();
    if (IsKeyDown(KEY_D))
        camera.target.x += CAMERA_SPEED * GetFrameTime();

    // Zoom
    camera.zoom += GetMouseWheelMove() * 0.1f;
    if (camera.zoom < 0.25f)
        camera.zoom = 0.25f;
    if (camera.zoom > 3.0f)
        camera.zoom = 3.0f;
}

void MapEditorScreen::DrawLoadDialog()
{
    // Semi-transparent overlay
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

    // Dialog box
    Rectangle dialogBox = {
        (float)(GetScreenWidth() / 2 - 300),
        (float)(GetScreenHeight() / 2 - 250),
        600, 500};
    DrawRectangleRounded(dialogBox, 0.1f, 10, WHITE);
    DrawRectangleRoundedLines(dialogBox, 0.1f, 10, DARKGRAY);

    // Title
    DrawText("Load Map", (int)(dialogBox.x + 20), (int)(dialogBox.y + 20), 28, BLACK);
    DrawText("Select a map to load:", (int)(dialogBox.x + 20), (int)(dialogBox.y + 60), 18, DARKGRAY);

    // Map list area
    Rectangle listArea = {
        dialogBox.x + 20,
        dialogBox.y + 90,
        560, 300};

    // List background
    DrawRectangleRec(listArea, LIGHTGRAY);
    DrawRectangleLines((int)listArea.x, (int)listArea.y, (int)listArea.width, (int)listArea.height, GRAY);

    // Scissor mode for scrolling
    BeginScissorMode((int)listArea.x, (int)listArea.y, (int)listArea.width, (int)listArea.height);

    // Draw map list
    const float ITEM_HEIGHT = 40.0f;
    const float PADDING = 5.0f;

    for (int i = 0; i < availableMaps.size(); i++)
    {
        Rectangle itemRect = {
            listArea.x + PADDING,
            listArea.y + PADDING + i * (ITEM_HEIGHT + PADDING) - mapListScrollY,
            listArea.width - 2 * PADDING,
            ITEM_HEIGHT};

        // Skip items that are not visible
        if (itemRect.y + itemRect.height < listArea.y || itemRect.y > listArea.y + listArea.height)
        {
            continue;
        }

        // Item background
        Color bgColor = (i == selectedMapIndex) ? BLUE : WHITE;
        Color textColor = (i == selectedMapIndex) ? WHITE : BLACK;

        DrawRectangleRec(itemRect, bgColor);
        DrawRectangleLines((int)itemRect.x, (int)itemRect.y, (int)itemRect.width, (int)itemRect.height, GRAY);

        // Map name
        DrawText(availableMaps[i].c_str(),
                 (int)(itemRect.x + 10),
                 (int)(itemRect.y + 10),
                 20, textColor);

        // Map info (if available)
        std::string mapInfo = "Mario Level " + std::to_string(i);
        DrawText(mapInfo.c_str(),
                 (int)(itemRect.x + 10),
                 (int)(itemRect.y + 25),
                 14, Fade(textColor, 0.7f));

        // Handle click
        if (CheckCollisionPointRec(GetMousePosition(), itemRect) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            selectedMapIndex = i;
            strcpy(mapNameBuffer, availableMaps[i].c_str());
        }
    }

    EndScissorMode();

    // Scrollbar (simple)
    if (availableMaps.size() * (ITEM_HEIGHT + PADDING) > listArea.height)
    {
        float scrollBarHeight = listArea.height * listArea.height / (availableMaps.size() * (ITEM_HEIGHT + PADDING));
        float scrollBarY = listArea.y + (mapListScrollY / (availableMaps.size() * (ITEM_HEIGHT + PADDING) - listArea.height)) * (listArea.height - scrollBarHeight);

        Rectangle scrollBar = {listArea.x + listArea.width - 10, scrollBarY, 10, scrollBarHeight};
        DrawRectangleRec(scrollBar, DARKGRAY);
    }

    // Buttons
    Rectangle loadBtn = {dialogBox.x + 150, dialogBox.y + 420, 120, 40};
    Rectangle cancelBtn = {dialogBox.x + 330, dialogBox.y + 420, 120, 40};

    // Load button
    bool canLoad = (selectedMapIndex >= 0 && selectedMapIndex < availableMaps.size());
    Color loadBtnColor = canLoad ? GREEN : GRAY;

    DrawRectangleRec(loadBtn, loadBtnColor);
    DrawRectangleLines((int)loadBtn.x, (int)loadBtn.y, (int)loadBtn.width, (int)loadBtn.height, DARKGRAY);
    DrawText("Load", (int)(loadBtn.x + 40), (int)(loadBtn.y + 12), 20, WHITE);

    if (canLoad && CheckCollisionPointRec(GetMousePosition(), loadBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        std::string selectedMap = availableMaps[selectedMapIndex];
        std::cout << "Loading map: " << selectedMap << std::endl;

        // TODO: Actually load the map
        LoadMap(selectedMap);

        showLoadDialog = false;
        selectedMapIndex = -1;
    }

    // Cancel button
    DrawRectangleRec(cancelBtn, RED);
    DrawRectangleLines((int)cancelBtn.x, (int)cancelBtn.y, (int)cancelBtn.width, (int)cancelBtn.height, DARKGRAY);
    DrawText("Cancel", (int)(cancelBtn.x + 30), (int)(cancelBtn.y + 12), 20, WHITE);

    if (CheckCollisionPointRec(GetMousePosition(), cancelBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        showLoadDialog = false;
        selectedMapIndex = -1;
    }

    // Instructions
    DrawText("Double-click to load, or use Load button",
             (int)(dialogBox.x + 20), (int)(dialogBox.y + 470), 14, GRAY);
}

void MapEditorScreen::LoadAvailableMaps()
{
    availableMaps.clear();

    const std::string mapsDirectory = "resources/maps/";

    try
    {
        // Scan directory for JSON files
        if (std::filesystem::exists(mapsDirectory))
        {
            for (const auto &entry : std::filesystem::directory_iterator(mapsDirectory))
            {
                if (entry.is_regular_file() && entry.path().extension() == ".json")
                {
                    std::string filename = entry.path().stem().string(); // Filename without extension
                    availableMaps.push_back(filename);
                }
            }
        }

        // Add some default maps if directory scanning fails
        if (availableMaps.empty())
        {
            availableMaps = {"map0", "map1", "map2", "map3", "map4"};
        }

        std::sort(availableMaps.begin(), availableMaps.end());
    }
    catch (const std::exception &e)
    {
        std::cout << "Error loading maps: " << e.what() << std::endl;
        // Fallback to hardcoded list
        availableMaps = {"map0", "map1", "map2", "map3", "map4"};
    }

    std::cout << "Found " << availableMaps.size() << " available maps" << std::endl;
}

void MapEditorScreen::HandleUIEvents()
{
    if (showLoadDialog)
    {
        // Handle scrolling in map list
        float wheelMove = GetMouseWheelMove();
        if (wheelMove != 0)
        {
            mapListScrollY -= wheelMove * 30;

            float maxScroll = std::max(0.0f, availableMaps.size() * 45.0f - 300.0f);
            if (mapListScrollY < 0)
                mapListScrollY = 0;
            if (mapListScrollY > maxScroll)
                mapListScrollY = maxScroll;
        }

        // Handle ESC to close dialog
        if (IsKeyPressed(KEY_ESCAPE))
        {
            showLoadDialog = false;
            selectedMapIndex = -1;
        }

        // Handle double-click to load
        static float lastClickTime = 0;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            float currentTime = GetTime();
            if (currentTime - lastClickTime < 0.3f && selectedMapIndex >= 0)
            {
                // Double click detected
                std::string selectedMap = availableMaps[selectedMapIndex];
                std::cout << "Double-click loading map: " << selectedMap << std::endl;

                LoadMap(selectedMap);

                showLoadDialog = false;
                selectedMapIndex = -1;
            }
            lastClickTime = currentTime;
        }
    }
}

