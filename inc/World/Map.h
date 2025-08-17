#pragma once
#include "raylib.h"
#include "Drawable.h"
#include "Object.h"
#include "Tile.h"
#include <vector>
#include "ResrcManager.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <memory>

#include "../inc/Block/Block.h"
#include "../inc/Block/QuestionBlock.h"
#include "../inc/Block/CloudBlock.h"
#include "../inc/Block/EyesOpenedBlock.h"
#include "../inc/Block/EyesClosedBlock.h"
#include "../inc/Block/GlassBlock.h"
#include "../inc/Block/WoodBlock.h"
#include "../inc/Enemy/Enemy.h"
#include "../inc/Enemy/Goomba.h"
#include "../inc/Enemy/GreenKoopa.h"
#include "../inc/Enemy/BuzzyBeetle.h"
#include "../inc/Enemy/Rex.h"
#include "../inc/Enemy/FlyingGoomba.h"
#include "../inc/Enemy/RedKoopa.h"
#include "../inc/Item/Coin.h"
#include "../inc/Item/Star.h"
#include "../inc/Item/Mushroom.h"
#include "../inc/Item/OneUpMushroom.h"
#include "../inc/Item/ThreeUpMoon.h"
#include "../inc/Item/YoshiCoin.h"
#include "../inc/Item/CourseClearToken.h"
#include "../inc/Item/FireFlower.h"
#include "../inc/Enemy/Bob-omb.h"
#include "../inc/Enemy/BulletBill.h"
#include "../inc/Enemy/PiranhaPlant.h"
#include "../inc/Enemy/YellowKoopa.h"
#include "../inc/Enemy/BanzaiBill.h"
#include "../inc/Enemy/JumpingPiranha.h"
#include <regex>

class Map : public Drawable {
    private:
        int currentMap;
        float width;
        const float height = 900.0f; 
        float currBackgroundStarX;
        Texture2D background;
        std::vector<Tile*> interactiveTiles;
        std::vector<Tile*> nonInteractiveTiles;
        std::vector<Block*> blocks;
        std::vector<Enemy*> enemies;
        std::vector<std::shared_ptr<Item>> interactiveItems;
    public:
        Map();
        ~Map();

        std::vector<Tile*>& getInteractiveTiles();
        std::vector<Block*>& getBlocks();
        std::vector<Enemy*>& GetEnemies();
        std::vector<std::shared_ptr<Item>>& GetInteractiveItems() { return interactiveItems; }
        float GetWidth() const;
        void nextMap();
        void LoadMap(int mapIndex);
        void SetMarioPositionForBosses(Vector2* player1Pos, Vector2* player2Pos = nullptr, bool isMultiplayer = false);
        void Draw() override;
        int ExtractMapIndex(const std::string& filename);
        void LoadFromJsonFile(const std::string& filepath);
        int getHeight() const { return 960; };
        int getWidth() const { return width; };
        void Clear();
        // void AddTile(int x, int y, int tileId);
        // void RemoveTile(int x, int y);
        // void AddBlock(int x, int y, int blockType);
        // void RemoveBlock(int x, int y);
        // void AddEnemy(int x, int y, int enemyType);
        // void RemoveEnemy(int x, int y);
        // void AddItem(int x, int y, int itemType);
        // void RemoveItem(int x, int y);
        
        // void SaveToFile(const std::string& filename);
        // void LoadFromFile(const std::string& filename);
};