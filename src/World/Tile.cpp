#include "../inc/World/Tile.h"

Tile::Tile(Vector2 pos,int map,int key):Object(pos,{32,32},WHITE)
{   
    this->key = key;
    std::string keyStr ="tile_" + std::to_string(key);
    this->sprite = &ResrcManager::GetInstance().getTexture(keyStr);
}

void Tile::Draw(){
    if(!this->sprite) return;
    DrawTexture(*this->sprite, this->pos.x, this->pos.y, WHITE);
}
