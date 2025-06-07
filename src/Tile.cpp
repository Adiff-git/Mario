#include "Tile.h"


Tile::Tile(Vector2 pos,int map,int key):Object(pos,{32,32},WHITE)
{
    std::string keyStr ="TILE_" + std::to_string(key);
    sprite = &ResrcManager::GetInstance().getTexture(keyStr);
}

void Tile::draw(){
    if(!sprite)
        return;
    DrawTexture(*sprite, pos.x, pos.y, WHITE);
}
