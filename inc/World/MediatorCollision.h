#pragma once
#include "Object.h"
#include "../inc/Character/Character.h"
#include "CollisionProbe.h"
#include "Tile.h"
#include "Fireball.h"
#include "BossFireball.h"
#include "Boss.h"
#include "../inc/Item/Item.h"
#include "../inc/Enemy/Enemy.h"
#include "../inc/Enemy/EnemyManager.h"
#include "../inc/Block/Block.h"
// class Mario; // Forward declaration
// class Tile; // Forward declaration
// class Fireball; // Forward declaration

class MediatorCollision
{
private:
    static int marioFireballHits;
    static const int maxFireballHits = 3;

    void HandleMarioWithTile(Character *&mario, Tile *&tile, CollisionType AtoB);
    void HandleFireballWithTile(Fireball *&fireball, Tile *&tile, CollisionType AtoB);
    void HandleItemWithTile(Item *&item, Tile *&tile, CollisionType AtoB);

    void HandleMarioWithEnemy(Character *&mario, Enemy *&enemy, CollisionType AtoB);
    // void HandleMarioWithBoss(Mario*& mario, Boss*& boss, CollisionType AtoB);
    void HandleEnemyWithFireball(Enemy *&enemy, Fireball *&fireball, CollisionType AtoB);
    // void HandleBossWithFireball(Boss*& boss, Fireball*& fireball, CollisionType AtoB);
    void HandleEnemyWithTile(Enemy *&enemy, Tile *tile, CollisionType AtoB);
    // void HandleBossWithTile(Boss*& boss, Tile*& tile, CollisionType AtoB);
    void HandleMarioWithBossFireball(Character *&mario, BossFireball *&bossFireball, CollisionType AtoB);

    void HandleMarioWithBlock(Character *&mario, Block *&block, CollisionType);

public:
    MediatorCollision() = default; // Ensure default constructor exists
    ~MediatorCollision() = default;

    void HandleCollision(Object *ObjectA, Object *ObjectB);
};
