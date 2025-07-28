#pragma once
#include "Object.h"
#include "Mario.h"
#include "CollisionProbe.h"
#include "Tile.h"
#include "Fireball.h"
#include "BossFireball.h"
#include "Boss.h"
#include "Item.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "Block.h"
// class Mario; // Forward declaration
// class Tile; // Forward declaration
// class Fireball; // Forward declaration

class MediatorCollision {
    private:
        static int marioFireballHits;
        static const int maxFireballHits = 3;
        
        void HandleMarioWithTile(Mario*& mario, Tile*& tile, CollisionType AtoB);
        void HandleFireballWithTile(Fireball*& fireball, Tile*& tile, CollisionType AtoB);
        void HandleItemWithTile(Item *& item, Tile *& tile, CollisionType AtoB);
        
        void HandleMarioWithEnemy(Mario*& mario, Enemy*& enemy, CollisionType AtoB);
        void HandleMarioWithBoss(Mario*& mario, Boss*& boss, CollisionType AtoB);
        void HandleEnemyWithFireball(Enemy*& enemy, Fireball*& fireball, CollisionType AtoB);
        void HandleBossWithFireball(Boss*& boss, Fireball*& fireball, CollisionType AtoB);
        void HandleEnemyWithTile(Enemy*& enemy, Tile* tile, CollisionType AtoB);
        void HandleMarioWithBossFireball(Mario*& mario, BossFireball*& bossFireball, CollisionType AtoB);

        void HandleMarioWithBlock(Mario* &mario, Block* &block, CollisionType);
        

    public:
        MediatorCollision() = default; // Ensure default constructor exists
        ~MediatorCollision() = default;

        void HandleCollision(Object* ObjectA, Object* ObjectB);
        

        
};
