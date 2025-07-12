#include "Map.h"

Map::Map()
{
    currBackgroundStarX = 0.0f;
    background = ResrcManager::GetInstance().getTexture("BACKGROUND_0");

    // Add all 8 items with distinct positions
    interactiveItems.push_back(std::make_shared<Coin>(Vector2{150, 800}));
    interactiveItems.push_back(std::make_shared<CourseClearToken>(Vector2{150, 800}));
    interactiveItems.push_back(std::make_shared<FireFlower>(Vector2{200, 800}));
    interactiveItems.push_back(std::make_shared<Mushroom>(Vector2{250, 800}));
    interactiveItems.push_back(std::make_shared<OneUpMushroom>(Vector2{300, 800}));
    interactiveItems.push_back(std::make_shared<Start>(Vector2{350, 800}));
    interactiveItems.push_back(std::make_shared<ThreeUpMoon>(Vector2{400, 800}));
    interactiveItems.push_back(std::make_shared<YoshiCoin>(Vector2{450, 800}));
}

Map::~Map()
{
    for (Tile* tile : interactiveTiles) {
        delete tile;
        tile = nullptr;
    }
    nonInteractiveTiles.clear();
}

std::vector<Tile*>& Map::getInteractiveTiles() {
    return interactiveTiles;
}

std::vector<std::shared_ptr<Item>>& Map::getInteractiveItems() {
    return interactiveItems;
}

float Map::GetWidth() const {
    return width;
}

void Map::nextMap() {
    // To be implemented
}

void Map::LoadMap(int mapIndex)
{
    std::string mapFileName = "resources/maps/map" + std::to_string(mapIndex) + ".json";
    std::ifstream file(mapFileName);
    if (!file) {
        std::cerr << "Could not open json file " << mapFileName << std::endl;
        return;
    }

    nlohmann::json mapJson;
    file >> mapJson;

    int width = mapJson["width"];
    int height = mapJson["height"];
    this->width = (float)width * 32.0f;
    std::vector<int> data = mapJson["layers"][0]["data"];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int tileId = data[y * width + x];
            if (tileId == 0) continue;
            Tile* tile = new Tile(Vector2{(float)x * 32, (float)y * 32}, mapIndex, tileId - 1);
            if (tileId == 1)
                nonInteractiveTiles.push_back(tile);
            else
                interactiveTiles.push_back(tile);
        }
    }
}

void Map::draw()
{
    for (auto tile : interactiveTiles) {
        tile->draw();
    }

    for (auto tile : nonInteractiveTiles) {
        tile->draw();
    }
}
