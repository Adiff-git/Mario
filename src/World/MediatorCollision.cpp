#include "../inc/World/MediatorCollision.h"
#include "../inc/Enemy/Enemy.h"
#include "../inc/Enemy/Goomba.h"
#include "../inc/Enemy/GreenKoopa.h"
#include "../inc/Enemy/BuzzyBeetle.h"
#include "../inc/Enemy/Rex.h"
#include "../inc/Enemy/FlyingGoomba.h"
#include "../inc/Enemy/PiranhaPlant.h"
#include "BossFireball.h"
#include "Boss.h"
#include <iostream>
#include <algorithm>
#include "../inc/Item/Coin.h"
#include "../inc/Item/Mushroom.h"
#include "../inc/Item/FireFlower.h"
#include "SoundManager.h"

void MediatorCollision::HandleMarioWithTile(Character *&mario, Tile *&tile, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;

    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        mario->SetPos(Vector2{mario->GetPos().x, tile->GetPos().y - mario->GetSize().y});
        mario->SetState(OBJECT_STATE_ON_GROUND);
        mario->SetVel(Vector2{mario->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_NORTH:
    {
        mario->SetPos(Vector2{mario->GetPos().x, tile->GetPos().y + tile->GetSize().y});
        mario->SetVel(Vector2{mario->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_EAST:
    {
        mario->SetPos(Vector2{tile->GetPos().x - mario->GetSize().x, mario->GetPos().y});
        mario->SetVel(Vector2{0, mario->GetVel().y});
        break;
    }
    case COLLISION_TYPE_WEST:
    {
        mario->SetPos(Vector2{tile->GetPos().x + tile->GetSize().x, mario->GetPos().y});
        mario->SetVel(Vector2{0, mario->GetVel().y});
        break;
    }
    default:
        break;
    }
}

void MediatorCollision::HandleFireballWithTile(Fireball *&fireball, Tile *&tile, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;
    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        fireball->SetPos(Vector2{fireball->GetPos().x, tile->GetPos().y - fireball->GetSize().y});
        fireball->SetVel(Vector2{fireball->GetVel().x, -500});
        break;
    }
    case COLLISION_TYPE_NORTH:
    {
        fireball->SetPos(Vector2{fireball->GetPos().x, tile->GetPos().y + tile->GetSize().y});
        fireball->SetVel(Vector2{fireball->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_EAST:
    {
        fireball->SetPos(Vector2{tile->GetPos().x - fireball->GetSize().x, fireball->GetPos().y});
        fireball->SetVel(Vector2{-fireball->GetVel().x, fireball->GetVel().y});
        if (fireball->GetCurrFrame() == 0)
        {
            fireball->setCurrFrame(3);
        }
        else
            fireball->setCurrFrame(fireball->GetCurrFrame() - 1);
        break;
    }
    case COLLISION_TYPE_WEST:
    {
        fireball->SetPos(Vector2{tile->GetPos().x + fireball->GetSize().x + tile->GetSize().x, fireball->GetPos().y});
        fireball->SetVel(Vector2{-fireball->GetVel().x, fireball->GetVel().y});
        if (fireball->GetCurrFrame() == 0)
        {
            fireball->setCurrFrame(3);
        }
        else
            fireball->setCurrFrame(fireball->GetCurrFrame() - 1);
        break;
    }
    }
}

void MediatorCollision::HandleItemWithTile(Item *&item, Tile *&tile, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;
    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        item->SetPos(Vector2{item->GetPos().x, tile->GetPos().y - item->GetSize().y});
        item->SetVel(Vector2{item->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_NORTH:
    {
        item->SetPos(Vector2{item->GetPos().x, tile->GetPos().y + tile->GetSize().y});
        item->SetVel(Vector2{item->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_EAST:
    {
        item->SetPos(Vector2{tile->GetPos().x - item->GetSize().x, item->GetPos().y});
        Vector2 vel = item->GetVel();
        vel.x = -abs(vel.x);
        item->SetVel(vel);
        item->SetDirection(DIRECTION_LEFT);
        if (item->GetCurrFrame() == 0)
            item->setCurrFrame(3);
        else
            item->setCurrFrame(item->GetCurrFrame() - 1);
        break;
    }
    case COLLISION_TYPE_WEST:
    {
        item->SetPos(Vector2{tile->GetPos().x + tile->GetSize().x, item->GetPos().y});
        Vector2 vel = item->GetVel();
        vel.x = abs(vel.x);
        item->SetVel(vel);
        item->SetDirection(DIRECTION_RIGHT);
        if (item->GetCurrFrame() == 0)
            item->setCurrFrame(3);
        else
            item->setCurrFrame(item->GetCurrFrame() - 1);
        break;
    }
    }
}

void MediatorCollision::HandleEnemyWithBlock(Enemy*& enemy, Block*& block, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;
    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        enemy->SetPos(Vector2{enemy->GetPos().x, block->GetPos().y - enemy->GetSize().y});
        enemy->SetState(OBJECT_STATE_ON_GROUND);
        enemy->SetVel(Vector2{enemy->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_NORTH:
    {
        enemy->SetPos(Vector2{enemy->GetPos().x, block->GetPos().y + block->GetSize().y});
        enemy->SetVel(Vector2{enemy->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_EAST:
    {
        enemy->SetPos(Vector2{block->GetPos().x - enemy->GetSize().x, enemy->GetPos().y});
        enemy->SetVel(Vector2{-enemy->GetVel().x, enemy->GetVel().y});
        enemy->SetDirection(DIRECTION_LEFT);
        break;
    }
    case COLLISION_TYPE_WEST:
    {
        enemy->SetPos(Vector2{block->GetPos().x + block->GetSize().x, enemy->GetPos().y});
        enemy->SetVel(Vector2{-enemy->GetVel().x, enemy->GetVel().y});
        enemy->SetDirection(DIRECTION_RIGHT);
        break;
    }
    }
}

void MediatorCollision::HandleItemWithBlock(Item*& item, Block*& block, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;
    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        item->SetPos(Vector2{item->GetPos().x, block->GetPos().y - item->GetSize().y});
        item->SetVel(Vector2{item->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_NORTH:
    {
        item->SetPos(Vector2{item->GetPos().x, block->GetPos().y + block->GetSize().y});
        item->SetVel(Vector2{item->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_EAST:
    {
        item->SetPos(Vector2{block->GetPos().x - item->GetSize().x, item->GetPos().y});
        Vector2 vel = item->GetVel();
        vel.x = -abs(vel.x);
        item->SetVel(vel);
        item->SetDirection(DIRECTION_LEFT);
        if (item->GetCurrFrame() == 0)
            item->setCurrFrame(3);
        else
            item->setCurrFrame(item->GetCurrFrame() - 1);
        break;
    }
    case COLLISION_TYPE_WEST:
    {
        item->SetPos(Vector2{block->GetPos().x + block->GetSize().x, item->GetPos().y});
        Vector2 vel = item->GetVel();
        vel.x = abs(vel.x);
        item->SetVel(vel);
        item->SetDirection(DIRECTION_RIGHT);
        if (item->GetCurrFrame() == 0)
            item->setCurrFrame(3);
        else
            item->setCurrFrame(item->GetCurrFrame() - 1);
        break;
    }
    }
}

void MediatorCollision::HandleCollision(Object *ObjectA, Object *ObjectB)
{
    Character* marioA = dynamic_cast<Character*>(ObjectA);
    Character* marioB = dynamic_cast<Character*>(ObjectB);

    Fireball* fireballA = dynamic_cast<Fireball*>(ObjectA);
    Fireball* fireballB = dynamic_cast<Fireball*>(ObjectB);

    Tile* tileA = dynamic_cast<Tile*>(ObjectA);
    Tile* tileB = dynamic_cast<Tile*>(ObjectB);

    Enemy* enemyA = dynamic_cast<Enemy*>(ObjectA);
    Enemy* enemyB = dynamic_cast<Enemy*>(ObjectB);

    Item* itemA = dynamic_cast<Item*>(ObjectA);
    Item* itemB = dynamic_cast<Item*>(ObjectB);

    Block* blockA = dynamic_cast<Block*>(ObjectA);
    Block* blockB = dynamic_cast<Block*>(ObjectB);

    BossFireball *bossFireballA = dynamic_cast<BossFireball *>(ObjectA);
    BossFireball *bossFireballB = dynamic_cast<BossFireball *>(ObjectB);
    
    // Mario vs Tile
    if ((marioA && tileB) || (marioB && tileA)) {
        Character* mario = marioA ? marioA : marioB;
        Tile* tile = tileA ? tileA : tileB;
        CollisionType type = mario->checkCollisionType(*tile);
        HandleMarioWithTile(mario, tile, type);
    }
    else if ((fireballA && tileB) || (fireballB && tileA))
    {
        Fireball *fireball = fireballA ? fireballA : fireballB;
        Tile *tile = tileA ? tileA : tileB;
        CollisionType type = fireball->checkCollisionType(*tile);
        HandleFireballWithTile(fireball, tile, type);
    }
    else if ((itemA && tileB) || (itemB && tileA))
    {
        Item *item = itemA ? itemA : itemB;
        Tile *tile = tileA ? tileA : tileB;
        CollisionType type = item->checkCollisionType(*tile);
        HandleItemWithTile(item, tile, type);
    }
    // Mario vs Item
    else if ((marioA && itemB) || (marioB && itemA)) {
        Character* mario = marioA ? marioA : marioB;
        Item* item = itemA ? itemA : itemB;
        if (item->canBeCollected() && item->checkCollision(*mario) == COLLISION_TYPE_COLLIDED) {
            item->updateMario(*mario);
            item->playCollisionSound();
            if (dynamic_cast<Mushroom*>(item)) {
                SoundManager::GetInstance().PlaySound("POWER_UP_APPEARS");
            } else if (dynamic_cast<FireFlower*>(item)) {
                SoundManager::GetInstance().PlaySound("POWER_UP_APPEARS");
            } else {
                SoundManager::GetInstance().PlaySound("COIN_COLLECTION");
            } 
        }
    }
    // Mario vs Enemy
    else if ((marioA && enemyB) || (marioB && enemyA)) {
        Character* mario = marioA ? marioA : marioB;
        Enemy* enemy = enemyA ? enemyA : enemyB;
        CollisionType type = mario->checkCollisionType(*enemy);
        HandleMarioWithEnemy(mario, enemy, type);
    }
    else if ((enemyA && tileB) || (enemyB && tileA))
    {
        Enemy *enemy = enemyA ? enemyA : enemyB;
        Tile *tile = tileA ? tileA : tileB;
        CollisionType type = enemy->checkCollisionType(*tile);
        HandleEnemyWithTile(enemy, tile, type);
    }
    else if ((enemyA && fireballB) || (enemyB && fireballA))
    {
        Enemy *enemy = enemyA ? enemyA : enemyB;
        Fireball *fireball = fireballA ? fireballA : fireballB;
        CollisionType type = enemy->checkCollisionType(*fireball);
        HandleEnemyWithFireball(enemy, fireball, type);
    }
    // Mario vs block
    else if((marioA && blockB) || (blockA && marioB)){
        Block* block = blockA ? blockA : blockB;
        Character* mario = marioA ? marioA :marioB;
        CollisionType type = mario->checkCollisionType(*block);
        HandleMarioWithBlock(mario, block, type);
    }
    else if ((marioA && bossFireballB) || (marioB && bossFireballA))
    {
        Character *mario = marioA ? marioA : marioB;
        BossFireball *bossFireball = bossFireballA ? bossFireballA : bossFireballB;
        CollisionType type = mario->checkCollisionType(*bossFireball);
        HandleMarioWithBossFireball(mario, bossFireball, type);
    }
    else if((enemyA && blockB) || (blockA && enemyB))
    {
        Enemy *enemy = enemyA ? enemyA : enemyB;
        Block *block = blockA ? blockA : blockB;
        CollisionType type = enemy->checkCollisionType(*block);
        HandleEnemyWithBlock(enemy, block, type);
    }
    else if((itemA && blockB) || (blockA && itemB))
    {
        Item *item = itemA ? itemA : itemB;
        Block *block = blockA ? blockA : blockB;
        CollisionType type = item->checkCollisionType(*block);
        HandleItemWithBlock(item, block, type);
    }
}

void MediatorCollision::HandleMarioWithBlock(Character* &mario, Block* &block, CollisionType type){
    if(type == COLLISION_TYPE_NONE) return;
    switch(type){
        case COLLISION_TYPE_SOUTH:{
            switch(block->GetBlockType())
            {
            case BLOCK_QUESTION:
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y - mario->GetSize().y});
                mario->SetState(OBJECT_STATE_ON_GROUND);
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                break;
            case BLOCK_CLOUD:
                if(mario->GetState() == OBJECT_STATE_FALLING)
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y - mario->GetSize().y});
                mario->SetState(OBJECT_STATE_ON_GROUND);
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                break;
            case BLOCK_GLASS:
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y - mario->GetSize().y});
                mario->SetState(OBJECT_STATE_ON_GROUND);
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                break;
            case BLOCK_WOOD:
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y - mario->GetSize().y});
                mario->SetState(OBJECT_STATE_ON_GROUND);
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                break;
            case BLOCK_EYES_OPENED:
                if(!block->isHit()){
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y - mario->GetSize().y});
                mario->SetState(OBJECT_STATE_ON_GROUND);
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                }
                break;
            case BLOCK_EYES_CLOSED:
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y - mario->GetSize().y});
                mario->SetState(OBJECT_STATE_ON_GROUND);
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                break;
            default:
                break;
            };
            break;
        }
        case COLLISION_TYPE_NORTH:{
            switch(block->GetBlockType())
            {
            case BLOCK_QUESTION:
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y + block->GetSize().y});
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                break;
            case BLOCK_CLOUD:
                break;
            case BLOCK_GLASS:
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y + block->GetSize().y});
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                break;
            case BLOCK_WOOD:
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y + block->GetSize().y});
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                break;
            case BLOCK_EYES_OPENED:
                if(!block->isHit())
                {
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y + block->GetSize().y});
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                }
                break;
            case BLOCK_EYES_CLOSED:
                mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y + block->GetSize().y});
                mario->SetVel(Vector2{mario->GetVel().x, 0});
                break;
            default:
                break;
            };
            break;
        }
        case COLLISION_TYPE_EAST:{
            switch(block->GetBlockType())
            {
            case BLOCK_QUESTION:
                mario->SetPos(Vector2{block->GetPos().x - mario->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                break;
            case BLOCK_CLOUD:
                break;
            case BLOCK_GLASS:
                mario->SetPos(Vector2{block->GetPos().x - mario->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                break;
            case BLOCK_WOOD:
                mario->SetPos(Vector2{block->GetPos().x - mario->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                break;
            case BLOCK_EYES_OPENED:
                if(!block->isHit()){
                mario->SetPos(Vector2{block->GetPos().x - mario->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                }
                break;
            case BLOCK_EYES_CLOSED:
                mario->SetPos(Vector2{block->GetPos().x - mario->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                break;
            default:
                break;
            };
            break;
        }
        case COLLISION_TYPE_WEST:{
            switch(block->GetBlockType())
            {
            case BLOCK_QUESTION:
                mario->SetPos(Vector2{block->GetPos().x + block->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                break;
            case BLOCK_CLOUD:
                break;
            case BLOCK_GLASS:
                mario->SetPos(Vector2{block->GetPos().x + block->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                break;
            case BLOCK_WOOD:
                mario->SetPos(Vector2{block->GetPos().x + block->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                break;
            case BLOCK_EYES_OPENED:
                if(!block->isHit()){
                mario->SetPos(Vector2{block->GetPos().x + block->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                }
                break;
            case BLOCK_EYES_CLOSED:
                mario->SetPos(Vector2{block->GetPos().x + block->GetSize().x, mario->GetPos().y});
                mario->SetVel(Vector2{0, mario->GetVel().y});
                break;
            default:
                break;
            };
            break;
        }
    }
}

void MediatorCollision::HandleEnemyWithTile(Enemy *&enemy, Tile *tile, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;
    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        enemy->SetPos(Vector2{enemy->GetPos().x, tile->GetPos().y - enemy->GetSize().y});
        enemy->SetState(OBJECT_STATE_ON_GROUND);
        enemy->SetVel(Vector2{enemy->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_NORTH:
    {
        enemy->SetPos(Vector2{enemy->GetPos().x, tile->GetPos().y + tile->GetSize().y});
        enemy->SetVel(Vector2{enemy->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_EAST:
    {
        enemy->SetPos(Vector2{tile->GetPos().x - enemy->GetSize().x, enemy->GetPos().y});
        enemy->SetVel(Vector2{-enemy->GetVel().x, enemy->GetVel().y});
        enemy->SetDirection(DIRECTION_LEFT);
        break;
    }
    case COLLISION_TYPE_WEST:
    {
        enemy->SetPos(Vector2{tile->GetPos().x + tile->GetSize().x, enemy->GetPos().y});
        enemy->SetVel(Vector2{-enemy->GetVel().x, enemy->GetVel().y});
        enemy->SetDirection(DIRECTION_RIGHT);
        break;
    }
    }
}

void MediatorCollision::HandleMarioWithEnemy(Character*& mario, Enemy*& enemy, CollisionType AtoB) {
    if (AtoB == COLLISION_TYPE_NONE) {
        return;
    }

    Boss *boss = dynamic_cast<Boss *>(enemy);
    if (boss)
    {
        if (boss->IsDead())
        {
            return;
        }
        switch (AtoB)
        {
        case COLLISION_TYPE_SOUTH:
        {
            mario->SetVel(Vector2{mario->GetVel().x, -300.0f});
            boss->OnHitByFireball();
            std::cout << "[DEBUG] Boss hit by Mario's stomp! Hit count: " << boss->GetHitCount() << "/10" << std::endl;
            break;
        }
        case COLLISION_TYPE_EAST:
        case COLLISION_TYPE_WEST:
        case COLLISION_TYPE_NORTH:
        {
            std::cout << "Boss damages Mario on side collision!" << std::endl;
            mario->BeHit();
            break;
        }
        }
        return;
    }

    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        mario->SetVel(Vector2{mario->GetVel().x, -300.0f});
        SoundManager::GetInstance().PlaySound("ENEMY_DEATH");
        Rex *rex = dynamic_cast<Rex *>(enemy);
        if (rex)
        {
            rex->OnHit();
            if (rex->GetHitCount() >= 2)
            {
                rex->CreateDeathEffect();
                rex->CreateScoreEffect(100); // Thêm dòng này để tạo text cộng điểm
                rex->SetState(OBJECT_STATE_DYING);
            }
        }
        else
        {
            enemy->CreateDeathEffect();
            enemy->CreateScoreEffect(100); // Thêm dòng này để tạo text cộng điểm
            enemy->SetHitByFireball(true);
            enemy->SetState(OBJECT_STATE_DYING);
            RedKoopa *redKoopa = dynamic_cast<RedKoopa *>(enemy);
            if (redKoopa)
            {
                float marioX = mario->GetPos().x;
                float koopaX = redKoopa->GetPos().x;
                bool fromLeft = (marioX < koopaX);
                redKoopa->OnHit(fromLeft);
                redKoopa->CreateDeathEffect();
                redKoopa->CreateScoreEffect(100); // Thêm dòng này để tạo text cộng điểm
                redKoopa->SetState(OBJECT_STATE_DYING);
            }
        }
        break;
    }
    case COLLISION_TYPE_EAST:
    case COLLISION_TYPE_WEST:
    case COLLISION_TYPE_NORTH:
    {
        mario->BeHit();
        break;
    }
    }
}

void MediatorCollision::HandleEnemyWithFireball(Enemy *&enemy, Fireball *&fireball, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
    {
        return;
    }

    Boss *boss = dynamic_cast<Boss *>(enemy);
    if (boss)
    {
        if (boss->IsDead())
        {
            fireball->SetState(OBJECT_STATE_DEAD);
            return;
        }
        boss->OnHitByFireball();
        fireball->SetState(OBJECT_STATE_DEAD);
        std::cout << "[DEBUG] Boss hit by Mario's Fireball! Hit count: " << boss->GetHitCount() << "/10" << std::endl;
        return;
    }

    if (dynamic_cast<BuzzyBeetle *>(enemy))
    {
        std::cout << "BuzzyBeetle is immune to fireball!" << std::endl;
        return;
    }
    enemy->CreateDeathEffect();
    enemy->CreateScoreEffect(100); // Thêm dòng này để tạo text cộng điểm
    enemy->SetState(OBJECT_STATE_DYING);
    std::cout << "Enemy dies by fireball" << std::endl;
}

void MediatorCollision::HandleMarioWithBossFireball(Character *&mario, BossFireball *&bossFireball, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;

    std::cout << "BossFireball collision detected! Type: " << AtoB << std::endl;

    if (AtoB != COLLISION_TYPE_NONE)
    {
        mario->BeHit();
        bossFireball->SetState(OBJECT_STATE_DEAD);
        std::cout << "Mario hit by BossFireball - dies instantly!" << std::endl;
    }
}