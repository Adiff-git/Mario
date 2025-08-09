#pragma once
#include "raylib.h"
#include "../inc/Item/Item.h"
#include "../inc/World/GameClock.h"
#include "../inc/World/Tile.h"
#include "../inc/World/Map.h"
#include "ResrcManager.h"
#include "../inc/World/MediatorCollision.h"
#include "../inc/Item/CourseClearToken.h"
#include "../inc/Item/FireFlower.h"
#include "../inc/Item/Coin.h"
#include "../inc/Screen/CharacterSelectScreen.h"
class Character;
class GameScreen;
enum class GameState {
    GAME_PLAYING,
    GAME_COMPLETED,
    GAME_OVER,
    GAME_RESET
};

class GameWorld {
    private:
        static constexpr float gravity = 200;
        Texture2D background;
        Map map;
        float BGpos = 0.0f;// 

        MediatorCollision mediatorCollision;
        

        Camera2D camera;
        
        GameState gameState;
        GameScreen* gameScreen;

        bool isMultiplayer;
        CharacterType player1Character;
        CharacterType player2Character;
        
        // Map and difficulty settings
        int selectedMapId;
        float enemySpeedMultiplier;

        std::vector<Tile*> &interactiveTiles;
    public:

        Character* player1;
        Character* player2;

        GameWorld();
        GameWorld(int level, GameScreen* gameScreen);
        GameWorld(int MapID, GameScreen* gameScreen, bool multiplayer, CharacterType p1Type, CharacterType p2Type); 
        GameWorld(int MapID, GameScreen* gameScreen, bool multiplayer, CharacterType p1Type, CharacterType p2Type, float speedMultiplier);
        ~GameWorld();

        void UpdateWorld();
        void DrawWorld();
        bool IsCompleted();
        GameState GetGameState() ;

        Map* GetMap();

        static const float GetGravity();
        static void Init();
        friend class GameScreen;

        Character* GetPlayer1();
        Character* GetPlayer2();
        bool IsMultiplayer() ;
        
        // Get enemy speed multiplier
        float GetEnemySpeedMultiplier() const { return enemySpeedMultiplier; }


        void CollectWorldData(GameSaveData& saveData);
        void ApplyLoadedData(const GameSaveData& saveData);
        GameSaveData CollectSaveData();
        
        friend class GameScreen;

        
        struct SpatialCell {
            std::vector<Block*> blocks;
            std::vector<Enemy*> enemies;
            std::vector<std::shared_ptr<Item>> items;
            std::vector<Tile*> tiles;
        };
    
        static constexpr int CELL_SIZE = 256;
        static constexpr int ACTIVE_MARGIN = 160; // mở rộng quanh viewport
        int gridCols = 0;
        int gridRows = 0;
        std::vector<std::vector<SpatialCell>> spatialGrid;
        
        void InitializeSpatialIndex();
        void RebuildSpatialIndex();
        void ClearSpatialGrid();
        template<typename TObj, typename FnPos>
        void InsertObjectToGrid(TObj* obj, FnPos getPosFn);
    
        void CollectActiveEntities(std::vector<Block*>& outBlocks,
                                   std::vector<Enemy*>& outEnemies,
                                   std::vector<std::shared_ptr<Item>>& outItems,
                                   std::vector<Tile*>& outTiles);
};