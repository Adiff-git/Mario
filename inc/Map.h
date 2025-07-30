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
#include "CourseClearToken.h"
#include "FireFlower.h"
#include "Block.h"
#include "QuestionBlock.h"
#include "CloudBlock.h"
#include "EyesOpenedBlock.h"
#include "EyesClosedBlock.h"
#include "GlassBlock.h"
#include "WoodBlock.h"
#include "Enemy.h"
#include "Goomba.h"
#include "GreenKoopa.h"
#include "BuzzyBeetle.h"
#include "Rex.h"
#include "FlyingGoomba.h"
#include "RedKoopa.h"
#include "Coin.h"
#include "Star.h"
#include "Mushroom.h"
#include "OneUpMushroom.h"
#include "ThreeUpMoon.h"
#include "YoshiCoin.h"
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
        void Draw() override;
        int ExtractMapIndex(const std::string& filename);
        void LoadFromJsonFile(const std::string& filepath);
        int getHeight() const { return 960; };
        int getWidth() const { return width; };
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