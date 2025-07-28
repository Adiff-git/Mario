#pragma once
#include "Object.h"
#include "Mario.h"
#include "CollisionProbe.h"
#include "Tile.h"
#include "Fireball.h"
#include "Item.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Block.h"

class MediatorCollision {
private:
    EnemyManager* enemyManager; // Con trỏ đến EnemyManager để truy cập enemies

    void HandleMarioWithTile(Mario*& mario, Tile*& tile, CollisionType AtoB);
    void HandleFireballWithTile(Fireball*& fireball, Tile*& tile, CollisionType AtoB);
    void HandleItemWithTile(Item*& item, Tile*& tile, CollisionType AtoB);
    void HandleMarioWithEnemy(Mario*& mario, Enemy*& enemy, CollisionType AtoB);
    void HandleEnemyWithFireball(Enemy*& enemy, Fireball*& fireball, CollisionType AtoB);
    void HandleEnemyWithTile(Enemy*& enemy, Tile* tile, CollisionType AtoB);
    void HandleMarioWithBlock(Mario*& mario, Block*& block, CollisionType);

public:
    MediatorCollision() = default; // Constructor mặc định để tránh lỗi
    MediatorCollision(EnemyManager* manager);
    ~MediatorCollision() = default;

    void HandleCollision(Object* ObjectA, Object* ObjectB);
    void UpdateEnemies();
};