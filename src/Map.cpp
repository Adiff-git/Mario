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

std::vector<Block*>& Map :: getBlocks(){
    return blocks;
}

std::vector<Enemy*>& Map::GetEnemies() {
    return enemies;
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
            else if(tileId == 2010) {
                blocks.push_back(new QuestionBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE, GIFT_NONE));
            }
            else if(tileId == 2011) {
                blocks.push_back(new QuestionBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE, GIFT_COIN));
            }
            else if(tileId == 2012) {
                blocks.push_back(new QuestionBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE, GIFT_FIRE_FLOWER));
            }
            else if (tileId == 200) {
                blocks.push_back(new CloudBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if (tileId == 202) {
                blocks.push_back(new WoodBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if (tileId == 203) {
                blocks.push_back(new GlassBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if (tileId == 204) {
                blocks.push_back(new EyesOpenedBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if (tileId == 205) {
                blocks.push_back(new EyesClosedBlock({(float)x * 32, (float)y * 32}, {32, 32}, WHITE));
            }
            else if(tileId == 206) {
                enemies.push_back(new Rex(Vector2{(float)x * 32 , (float)(y * 32 - 35)}));
            }
            else if(tileId == 207) {
                enemies.push_back(new GreenKoopa(Vector2{(float)x * 32 , (float)(y * 32 - 20)}));
            }
            else if(tileId == 208) {
                enemies.push_back(new BuzzyBeetle(Vector2{(float)x * 32, (float)y * 32}));
            }
            else if(tileId >= 100 && tileId <= 199) {
                interactiveTiles.push_back(new Tile(Vector2{(float)x * 32,(float)y * 32 },mapIndex ,tileId-100));
            }
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

    for (auto block : blocks) {
        block->Draw();
    }

    for(auto enemy : enemies) {
        enemy->Draw();
    }
}

Map::Map()
{   
    currBackgroundStarX = 0.0f;
    background = ResrcManager::GetInstance().getTexture("BACKGROUND_3");
    // Coin coin({200, 800});
    

}