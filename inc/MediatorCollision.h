#pragma once

#include "Object.h"
#include "Mario.h"
#include "CollisionProbe.h"
#include "Tile.h"
#include "Fireball.h"
#include "Enemy.h"
#include <vector>

class MediatorCollision {
private:
    void HandleMarioWithTile(Mario*& mario, Tile*& tile, CollisionType AtoB);
    void HandleFireballWithTile(Fireball*& fireball, Tile*& tile, CollisionType AtoB);
    
    std::vector<Enemy*> enemies;

public:
    MediatorCollision();
    ~MediatorCollision() = default;
    void HandleCollision(Object* ObjectA, Object* ObjectB);
    std::vector<Enemy*>& GetEnemies();
    void HandleEnemyWithTile(Enemy*& enemy, Tile* tile, CollisionType AtoB);

};