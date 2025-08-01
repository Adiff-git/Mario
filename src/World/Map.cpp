#include "../inc/World/Map.h"
#include "Boss.h"


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

std::vector<Block*>& Map :: getBlocks(){
    return blocks;
}

std::vector<Enemy*>& Map::GetEnemies() {
    return enemies;
}

void Map::SetMarioPositionForBosses(Vector2* marioPos) {
    for (Enemy* enemy : enemies) {
        Boss* boss = dynamic_cast<Boss*>(enemy);
        if (boss != nullptr) {
            boss->SetMarioPosition(marioPos);
        }
    }
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
            else if(tileId == 105) {
                blocks.push_back(new CloudBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if(tileId == 116) {
                blocks.push_back(new QuestionBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE, GIFT_NONE));
            }
            else if(tileId == 117) {
                blocks.push_back(new QuestionBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE, GIFT_COIN));
            }
            else if(tileId == 118) {
                blocks.push_back(new QuestionBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE, GIFT_FIRE_FLOWER));
            }

            else if (tileId == 121) {
                blocks.push_back(new WoodBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if (tileId == 112) {
                blocks.push_back(new GlassBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if (tileId == 108) {
                blocks.push_back(new EyesOpenedBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if (tileId == 107) {
                blocks.push_back(new EyesClosedBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if(tileId == 135) {
                enemies.push_back(new Rex(Vector2{(float)x * 32 , (float)(y * 32 - 35)}));
            }
            else if(tileId == 128) {
                enemies.push_back(new GreenKoopa(Vector2{(float)x * 32 , (float)(y * 32 - 20)}));
            }
            else if(tileId == 127) {
                enemies.push_back(new Goomba(Vector2{(float)x * 32 , (float)(y * 32 - 20)}));
            }
            else if(tileId == 125) {
                enemies.push_back(new BuzzyBeetle(Vector2{(float)x * 32 , (float)(y * 32 - 20)}));
            }
            else if(tileId == 126) {
                enemies.push_back(new FlyingGoomba(Vector2{(float)x * 32 , (float)(y * 32 - 20)}));
            }
            else if (tileId == 138) {
                // 1UpMushroom
                interactiveItems.push_back(std::make_shared<OneUpMushroom>(Vector2{(float)x * 32, (float)y * 32}));
            }
            else if (tileId == 139) {
                // 3UpMoon
                interactiveItems.push_back(std::make_shared<ThreeUpMoon>(Vector2{(float)x * 32, (float)y * 32}));
            }
            else if (tileId == 140) {
                // Coin
                interactiveItems.push_back(std::make_shared<Coin>(Vector2{(float)x * 32, (float)y * 32}));
            }
            else if (tileId == 141) {
                // FireFlower
                interactiveItems.push_back(std::make_shared<FireFlower>(Vector2{(float)x * 32, (float)y * 32}));
            }
            else if (tileId == 142) {
                // Mushroom
                interactiveItems.push_back(std::make_shared<Mushroom>(Vector2{(float)x * 32, (float)y * 32}));
            }
            else if (tileId == 143) {
                // Star
                interactiveItems.push_back(std::make_shared<Star>(Vector2{(float)x * 32, (float)y * 32}));
            }
            else if (tileId == 144) {
                // YoshiCoin
                interactiveItems.push_back(std::make_shared<YoshiCoin>(Vector2{(float)x * 32, (float)y * 32}));
            }
            else if(tileId <=104) {
                interactiveTiles.push_back(new Tile(Vector2{(float) x * 32,(float) y * 32 },mapIndex ,tileId-1));
            }else continue;
        }
    }
}

int Map :: ExtractMapIndex(const std::string& filename) {
    std::smatch match;
    std::regex pattern("map(\\d+)\\.json"); // tìm số sau 'map' và trước '.json'
    if (std::regex_search(filename, match, pattern)) {
        return std::stoi(match[1]);
    }
    return -1; // nếu không tìm thấy
}

void Map :: LoadFromJsonFile(const std::string& filepath){
     std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Could not open json file " << filepath << std::endl;
        return;
    }
     // Assuming mapIndex is determined from the filepath or some other logic
    nlohmann::json mapJson;
    file >> mapJson;
    int mapIndex = ExtractMapIndex(filepath);
    int width = mapJson["width"];
    int height = mapJson["height"];
    this->width = static_cast<float>(width * 32);
    int tileheight = mapJson["tileheight"];
    int tilewidth = mapJson["tilewidth"];
    std::vector<int> data = mapJson["layers"][0]["data"];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int tileId = data[y * width + x];
            if (tileId == 0)
                continue;
            else if(tileId == 105) {
                blocks.push_back(new CloudBlock({(float)x * tilewidth, (float)y * tileheight}, {32, 32}, WHITE));
            }
            else if(tileId == 116) {
                blocks.push_back(new QuestionBlock({(float)x * tilewidth, (float)y * tileheight}, {32, 32}, WHITE, GIFT_NONE));
            }
            else if(tileId == 117) {
                blocks.push_back(new QuestionBlock({(float)x * tilewidth, (float)y * tileheight}, {32, 32}, WHITE, GIFT_COIN));
            }
            else if(tileId == 118) {
                blocks.push_back(new QuestionBlock({(float)x * tilewidth, (float)y * tileheight}, {32, 32}, WHITE, GIFT_FIRE_FLOWER));
            }

            else if (tileId == 121) {
                blocks.push_back(new WoodBlock({(float)x * tilewidth, (float)y * tileheight}, {32, 32}, WHITE));
            }
            else if (tileId == 112) {
                blocks.push_back(new GlassBlock({(float)x * tilewidth, (float)y * tileheight}, {32, 32}, WHITE));
            }
            else if (tileId == 108) {
                blocks.push_back(new EyesOpenedBlock({(float)x * tilewidth, (float)y * tileheight}, {32, 32}, WHITE));
            }
            else if (tileId == 107) {
                blocks.push_back(new EyesClosedBlock({(float)x * tilewidth, (float)y * tileheight}, {32, 32}, WHITE));
            }
            else if(tileId == 135) {
                enemies.push_back(new Rex(Vector2{(float)x * tilewidth , (float)(y * tileheight - 35)}));
            }
            else if(tileId == 128) {
                enemies.push_back(new GreenKoopa(Vector2{(float)x * tilewidth , (float)(y * tileheight - 20)}));
            }
            else if(tileId == 127) {
                enemies.push_back(new Goomba(Vector2{(float)x * tilewidth , (float)(y * tileheight - 20)}));
            }
            else if(tileId == 125) {
                enemies.push_back(new BuzzyBeetle(Vector2{(float)x * tilewidth , (float)(y * tileheight - 20)}));
            }
            else if(tileId == 126) {
                enemies.push_back(new FlyingGoomba(Vector2{(float)x * tilewidth , (float)(y * tileheight - 20)}));
            }
            else if (tileId == 138) {
                // 1UpMushroom
                interactiveItems.push_back(std::make_shared<OneUpMushroom>(Vector2{(float)x * tilewidth, (float)y * tileheight}));
            }
            else if (tileId == 139) {
                // 3UpMoon
                interactiveItems.push_back(std::make_shared<ThreeUpMoon>(Vector2{(float)x * tilewidth, (float)y * tileheight}));
            }
            else if (tileId == 140) {
                // Coin
                interactiveItems.push_back(std::make_shared<Coin>(Vector2{(float)x * tilewidth, (float)y * tileheight}));
            }
            else if (tileId == 141) {
                // FireFlower
                interactiveItems.push_back(std::make_shared<FireFlower>(Vector2{(float)x * tilewidth, (float)y * tileheight}));
            }
            else if (tileId == 142) {
                // Mushroom
                interactiveItems.push_back(std::make_shared<Mushroom>(Vector2{(float)x * tilewidth, (float)y * tileheight}));
            }
            else if (tileId == 143) {
                // Star
                interactiveItems.push_back(std::make_shared<Star>(Vector2{(float)x * tilewidth, (float)y * tileheight}));
            }
            else if (tileId == 144) {
                // YoshiCoin
                interactiveItems.push_back(std::make_shared<YoshiCoin>(Vector2{(float)x * tilewidth, (float)y * tileheight}));
            }
            else if(tileId <=104) {
                interactiveTiles.push_back(new Tile(Vector2{(float) x * tilewidth,(float) y * tileheight }  ,mapIndex ,tileId-1));
            }else continue;
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

    for (auto block : blocks) {
        block->Draw();
    }

    for(auto enemy : enemies) {
        enemy->Draw();
    }
    for(auto const &item : interactiveItems) {
        item->Draw();
    }
}

Map::Map()
{   
    currBackgroundStarX = 0.0f;
    background = ResrcManager::GetInstance().getTexture("BACKGROUND_3");
    // Coin coin({200, 800});
    

}