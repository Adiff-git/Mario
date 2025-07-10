#include "Map.h"


Map::~Map()
{
    for (Tile* tile : interactiveTiles) {
        delete tile;
        tile = nullptr;
    }
    nonInteractiveTiles.clear();
}

std::vector<Tile *> &Map::getInteractiveTiles()
{
    
    return interactiveTiles;
}

std::vector<std::shared_ptr<Coin>> &Map::getInteractiveCoins()
{
    
    return interactiveCoins;
}

std::vector<std::shared_ptr<FireFlower>> &Map::getInteractiveFireFlowers()
{
    
    return interactiveFireFlowers;
}

std::vector<std::shared_ptr<CourseClearToken>>& Map::getInteractiveCourseClearTokens()
{
    return interactiveCourseClearTokens;
}

float Map::GetWidth() const
{
    return width;
}

void Map::nextMap()
{
    
}

void Map::LoadMap(int mapIndex)
{
    char* map = nullptr;
    std::string mapFileName = "resources/maps/map" +std::to_string(mapIndex) + ".json";
    std::ifstream file(mapFileName);
    if (!file) {
        std::cerr << "Could not open json file " << mapFileName << std::endl;
        return;
    }
    nlohmann::json mapJson;
    file >> mapJson;

    int width = mapJson["width"];
    int height = mapJson["height"];
    this->width = (float) width * 32.0f;
    int tilewidth = mapJson["tilewidth"];
    std::vector<int> data = mapJson["layers"][0]["data"];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int tileId = data[y * width + x];
            if (tileId == 0)
                continue;
            else if(tileId==1)
                nonInteractiveTiles.push_back(new Tile(Vector2{(float) x * 32,(float) y * 32 },mapIndex ,tileId-1));
            else interactiveTiles.push_back(new Tile(Vector2{(float) x * 32,(float) y * 32 },mapIndex ,tileId-1));
            }
        }
}

void Map::Draw()
{
    for (auto tile : interactiveTiles) {
        tile->Draw();
    }

    for (auto tile : nonInteractiveTiles) {
        tile->Draw();
    }
}

//===============================
//Nhi
Map::Map()
{
    currBackgroundStarX = 0.0f;
    background = ResrcManager::GetInstance().getTexture("BACKGROUND_0");

    // Coin
    auto coin = std::make_shared<Coin>(Vector2{200, 800});
    interactiveItems.push_back(coin);

    // Course Clear Token
    auto courseClearToken = std::make_shared<CourseClearToken>(Vector2{250, 800});
    interactiveItems.push_back(courseClearToken);

    // Fire Flower
    auto fireFlower = std::make_shared<FireFlower>(Vector2{300, 800});
    interactiveItems.push_back(fireFlower);

    // Mushroom
    auto mushroom = std::make_shared<Mushroom>(Vector2{350, 800});
    interactiveItems.push_back(mushroom);

    // One Up Mushroom
    auto oneUp = std::make_shared<OneUpMushroom>(Vector2{400, 800});
    interactiveItems.push_back(oneUp);

    // Star
    auto star = std::make_shared<Start>(Vector2{450, 800});
    interactiveItems.push_back(star);

    // Three Up Moon
    auto moon = std::make_shared<ThreeUpMoon>(Vector2{500, 800});
    interactiveItems.push_back(moon);

    // Yoshi Coin
    auto yoshi = std::make_shared<YoshiCoin>(Vector2{550, 800});
    interactiveItems.push_back(yoshi);
}
//======================
