#pragma once

#include "raylib.h"
#include "Drawable.h"
#include "Object.h"
#include "Tile.h"
#include "Item.h"
#include <vector>
#include "ResrcManager.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <memory>

#include "Coin.h"
#include "CourseClearToken.h"
#include "FireFlower.h"
#include "Mushroom.h"
#include "OneUpMushroom.h"
#include "Start.h"
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
    std::vector<std::shared_ptr<Item>> interactiveItems;

    void LoadFromJsonFile(const std::string& filename);

public:
    Map();
    ~Map();

    std::vector<Tile*>& getInteractiveTiles();
    std::vector<std::shared_ptr<Item>>& getInteractiveItems();

    float GetWidth() const;
    void nextMap();
    void LoadMap(int mapIndex);
    void draw() override;
};
