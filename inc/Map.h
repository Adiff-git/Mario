#pragma once
#include "raylib.h"
#include "Drawable.h"
#include "Object.h"
#include "Tile.h"
#include <vector>
#include "Block.h"
#include "ResrcManager.h"
#include "json.hpp"
#include <iostream>
#include "QuestionBlock.h"
#include "CloudBlock.h"
#include "WoodBlock.h"
#include "GlassBlock.h"
#include <fstream>

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
        void LoadFromJsonFile(const std::string& filename);
    public:
        Map();
        ~Map();
        std::vector<Tile*>& getInteractiveTiles();
        std::vector<Tile*>& getNonInteractiveTiles();
        float GetWidth() const;
        void nextMap();
        void LoadMap(int mapIndex);
        void draw() override;
        std::vector<Block*>& getBlocks();
};