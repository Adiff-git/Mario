#pragma once
#include "Screen.h"
#include "Button.h"
#include "raylib.h"
#include <vector>
#include "../inc/World/Map.h"
#include "../inc/Block/Block.h"
#include "../inc/Enemy/Enemy.h"
#include "../inc/Item/Item.h"

class Map;

enum class EditorMode {
    TILE_MODE,
    BLOCK_MODE,
    ENEMY_MODE,
    ITEM_MODE
};

struct TilesetTile {
    int tileId;
    Texture2D texture;
    Rectangle sourceRect;
};

class MapEditorScreen : public Screen {
private:

    void RemoveObjectAt(int tileX, int tileY, EditorMode mode);
    Block* CreateBlockByType(int tileId, Vector2 position);
    Enemy* CreateEnemyByType(int tileId, Vector2 position);
    std::shared_ptr<Item> CreateItemByType(int tileId, Vector2 position);
    void PlaceObject();
    void RemoveObject();
    void HandleMapInput();
    
    // Helper functions
    Vector2 ScreenToGrid(Vector2 screenPos) const;
    Vector2 GridToWorld(Vector2 gridPos) const;
    bool IsMouseInMainViewport() const;
    Map* currentMap;
    Camera2D camera;
    Vector2 mouseWorldPos;
    Button backToMenuButton2;
    Button backToMenuButton;
    Button loadMapButton;
    Button saveMapButton;
    Texture2D* backgroundTexture;
    
    EditorMode currentMode;
    int selectedTileId;
    int selectedBlockType;
    int selectedEnemyType;
    int selectedItemType;
    
    const float TILESET_PANEL_WIDTH = 300.0f;
    const float TOOLBAR_HEIGHT = 60.0f;
    const int GRID_SIZE = 32;
    
    bool showLoadDialog;
    bool editorActive = false;
    bool showSaveDialog;
    std::vector<std::string> availableMaps;
    int selectedMapIndex;
    float mapListScrollY;
    float tilesetScrollY;
    char mapNameBuffer[256];
    
    std::vector<TilesetTile> availableTiles;
    std::vector<TilesetTile> availableBlocks;
    std::vector<TilesetTile> availableEnemies;
    std::vector<TilesetTile> availableItems;


    // Map operations
    void LoadMap(const std::string& mapName);
    void NewMap();
    void SaveMap(const std::string& mapName);
    
    // UI Drawing
    void DrawMainEditor();
    void DrawMapGrid();
    void DrawMapContent();
    void DrawTilesetPanel();
    void DrawTilesetContent(Rectangle area);
    void DrawToolbar();
    void DrawCursorPreview();
    
    // Dialogs
    void DrawLoadDialog();
    void DrawSaveDialog();
    
    // Input handling
    void HandleInput();
    void HandleUIEvents();
    void UpdateCamera();
    int GetBlockTileId(Block* block);
    int GetEnemyTileId(Enemy* enemy);
    int GetItemTileId(Item* item);

    // Editor actions
    //void PlaceObject();
    //void RemoveObject();

    // Initialization
    void LoadAvailableMaps();
    void InitializeTilesets();
    
    // Helper functions
    Rectangle GetMainViewport() const;
    Rectangle GetTilesetPanel() const;
    //Vector2 ScreenToGrid(Vector2 screenPos) const;
    //Vector2 GridToWorld(Vector2 gridPos) const;
    
public:
    MapEditorScreen(ScreenController* screenController);
    ~MapEditorScreen() override;
    
    void Update() override;
    void Draw() override;
};