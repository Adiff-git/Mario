#pragma once

#include "Object.h"
#include "Mario.h"
#include "CollisionProbe.h"
#include "Tile.h"
#include "Fireball.h"
#include "Enemy.h"
#include <vector>
#include <iostream>
#include <algorithm>
class MediatorCollision {
private:
    void HandleMarioWithTile(Mario*& mario, Tile*& tile, CollisionType AtoB);
    void HandleFireballWithTile(Fireball*& fireball, Tile*& tile, CollisionType AtoB);
    void HandleMarioWithEnemy(Mario*& mario, Enemy*& enemy, CollisionType AtoB);
    void HandleEnemyWithFireball(Enemy*& enemy, Fireball*& fireball, CollisionType AtoB);
    void HandleEnemyWithTile(Enemy*& enemy, Tile* tile, CollisionType AtoB);
    std::vector<Enemy*> enemies;
public:
    MediatorCollision();
    ~MediatorCollision() = default;
    void HandleCollision(Object* ObjectA, Object* ObjectB);
    std::vector<Enemy*>& GetEnemies();
};