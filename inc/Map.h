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
        void LoadFromJsonFile(const std::string& filename);
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
        void SetMarioPositionForBosses(Vector2* marioPos);
        void Draw() override;
};