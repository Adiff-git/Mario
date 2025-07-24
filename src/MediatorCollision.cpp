#include "MediatorCollision.h"
#include "Enemy.h"
#include "Goomba.h"
#include "GreenKoopa.h"
#include "BuzzyBeetle.h"
#include "Rex.h"
#include "FlyingGoomba.h"
#include "PiranhaPlant.h"
#include <iostream>
#include <algorithm>
void MediatorCollision::HandleMarioWithTile(Mario* &mario, Tile * &tile, CollisionType AtoB)
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
            mario->SetVel(Vector2{0, mario->GetVel().y}); // Reverse the x velocity
            break;
        }
        
        case COLLISION_TYPE_WEST:
        {
            mario->SetPos(Vector2{tile->GetPos().x + tile->GetSize().x, mario->GetPos().y});
            mario->SetVel(Vector2{0, mario->GetVel().y}); // Reverse the x velocity
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
        fireball->SetVel(Vector2{-fireball->GetVel().x, fireball->GetVel().y}); // Reverse the x velocity
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
        fireball->SetVel(Vector2{-fireball->GetVel().x, fireball->GetVel().y}); // Reverse the x velocity
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
            // Đặt item lên trên tile và cho di chuyển sang phải
            std::cout << "==============================================================================" << std::endl;
            item->SetPos(Vector2{item->GetPos().x, tile->GetPos().y - item->GetSize().y});
            item->SetVel(Vector2{item->GetVel().x, 0});  // Cho phép item tiếp tục di chuyển
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
            
            // Đặt item sát bên trái tile, giữ nguyên Y (không thay đổi Y)
            item->SetPos(Vector2{tile->GetPos().x - item->GetSize().x, item->GetPos().y});
            // Đảo chiều vận tốc X, giữ nguyên vận tốc Y
            Vector2 vel = item->GetVel();
            vel.x = -abs(vel.x); // Đảm bảo đi sang trái
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
            
            // Đặt item sát bên phải tile, giữ nguyên Y (không thay đổi Y)
            item->SetPos(Vector2{tile->GetPos().x + tile->GetSize().x, item->GetPos().y});
            // Đảo chiều vận tốc X, giữ nguyên vận tốc Y
            Vector2 vel = item->GetVel();
            vel.x = abs(vel.x); // Đảm bảo đi sang phải
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



void MediatorCollision::HandleCollision(Object* ObjectA, Object* ObjectB)
{
    Mario* marioA = dynamic_cast<Mario*>(ObjectA);
    Mario* marioB = dynamic_cast<Mario*>(ObjectB);

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
    // Mario vs Tile
    if ((marioA && tileB) || (marioB && tileA)) {
        Mario* mario = marioA ? marioA : marioB;
        Tile* tile = tileA ? tileA : tileB;
        CollisionType type = mario->checkCollisionType(*tile);
        HandleMarioWithTile(mario, tile, type);
    }

    // Fireball vs Tile
    else if ((fireballA && tileB) || (fireballB && tileA)) {
        Fireball* fireball = fireballA ? fireballA : fireballB;
        Tile* tile = tileA ? tileA : tileB;
        CollisionType type = fireball->checkCollisionType(*tile);
        HandleFireballWithTile(fireball, tile, type);
    }

    // Item vs Tile
    else if ((itemA && tileB) || (itemB && tileA)) {
        Item* item = itemA ? itemA : itemB;
        Tile* tile = tileA ? tileA : tileB;
        CollisionType type = item->checkCollisionType(*tile);
        HandleItemWithTile(item, tile, type);
    }

    // Mario vs Item
    else if ((marioA && itemB) || (marioB && itemA)) {
        Mario* mario = marioA ? marioA : marioB;
        Item* item = itemA ? itemA : itemB;

        if (item->canBeCollected() && item->checkCollision(*mario) == COLLISION_TYPE_COLLIDED) {
            item->updateMario(*mario);
            item->playCollisionSound();
        }
    }

    // Mario vs Enemy
    else if ((marioA && enemyB) || (marioB && enemyA)) {
        Mario* mario = marioA ? marioA : marioB;
        Enemy* enemy = enemyA ? enemyA : enemyB;
        CollisionType type = mario->checkCollisionType(*enemy);
        HandleMarioWithEnemy(mario, enemy, type);
    }

    // Enemy vs Tile
    else if ((enemyA && tileB) || (enemyB && tileA)) {
        Enemy* enemy = enemyA ? enemyA : enemyB;
        Tile* tile = tileA ? tileA : tileB;
        CollisionType type = enemy->checkCollisionType(*tile);
        HandleEnemyWithTile(enemy, tile, type);
    }

    // Enemy vs Fireball
    else if ((enemyA && fireballB) || (enemyB && fireballA)) {
        Enemy* enemy = enemyA ? enemyA : enemyB;
        Fireball* fireball = fireballA ? fireballA : fireballB;
        CollisionType type = enemy->checkCollisionType(*fireball);
        HandleEnemyWithFireball(enemy, fireball, type);
    }

    // Mario vs block
    else if((marioA && blockB) || (blockA && marioB)){
        Block* block = blockA ? blockA : blockB;
        Mario* mario = marioA ? marioA :marioB;
        CollisionType type = mario->checkCollisionType(*block);
        HandleMarioWithBlock(mario, block, type);
    }
}

void MediatorCollision :: HandleMarioWithBlock(Mario* &mario, Block* &block, CollisionType type){
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
};

void MediatorCollision::HandleEnemyWithTile(Enemy*& enemy, Tile* tile, CollisionType AtoB) {
    if (AtoB == COLLISION_TYPE_NONE) return;
    switch (AtoB) {
        case COLLISION_TYPE_SOUTH: {
            enemy->SetPos(Vector2{enemy->GetPos().x, tile->GetPos().y - enemy->GetSize().y});
            enemy->SetState(OBJECT_STATE_ON_GROUND);
            enemy->SetVel(Vector2{enemy->GetVel().x, 0});
            break;
        }
        case COLLISION_TYPE_NORTH: {
            enemy->SetPos(Vector2{enemy->GetPos().x, tile->GetPos().y + tile->GetSize().y});
            enemy->SetVel(Vector2{enemy->GetVel().x, 0});
            break;
        }
        case COLLISION_TYPE_EAST: {
            enemy->SetPos(Vector2{tile->GetPos().x - enemy->GetSize().x, enemy->GetPos().y});
            enemy->SetVel(Vector2{-enemy->GetVel().x, enemy->GetVel().y});
            enemy->SetDirection(DIRECTION_LEFT);
            
            break;
        }
        case COLLISION_TYPE_WEST: {
            enemy->SetPos(Vector2{tile->GetPos().x + tile->GetSize().x, enemy->GetPos().y});
            enemy->SetVel(Vector2{-enemy->GetVel().x, enemy->GetVel().y});
            enemy->SetDirection(DIRECTION_RIGHT);
            
            break;
        }
    }
}

void MediatorCollision::HandleMarioWithEnemy(Mario*& mario, Enemy*& enemy, CollisionType AtoB) {
    if (AtoB == COLLISION_TYPE_NONE) {
        std::cout << "No collision between Mario and Enemy" << std::endl;
        return;
    }

    switch (AtoB) {
        case COLLISION_TYPE_SOUTH: {
            RedKoopa* redKoopa = dynamic_cast<RedKoopa*>(enemy);
            if (redKoopa) {
                float marioX = mario->GetPos().x;
                float koopaX = redKoopa->GetPos().x;
                bool fromLeft = (marioX < koopaX); 
                redKoopa->OnHit(fromLeft); 
                mario->SetVel(Vector2{mario->GetVel().x, -300.0f}); 
            } else {
                Rex* rex = dynamic_cast<Rex*>(enemy);
                if (rex) {
                    rex->OnHit(); 
                    mario->SetVel(Vector2{mario->GetVel().x, -300.0f});
                    if (rex->GetHitCount() >= 2) {
                        rex->SetState(OBJECT_STATE_DEAD);
                        // enemies.erase(std::remove(enemies.begin(), enemies.end(), rex), enemies.end());
                        // delete rex;
                        // rex = nullptr;
                    }
                } else {
                    
                    // enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
                    // delete enemy;
                    // enemy = nullptr;
                    enemy->SetState(OBJECT_STATE_DEAD);
                    mario->SetVel(Vector2{mario->GetVel().x, -300.0f});
                }
            }
            break;
        }
        //====================================================
        case COLLISION_TYPE_EAST:
        case COLLISION_TYPE_WEST:
        case COLLISION_TYPE_NORTH: {
            mario->BeHit();
            break;
        }
    }
}

void MediatorCollision::HandleEnemyWithFireball(Enemy* &enemy, Fireball* &fireball, CollisionType AtoB) {
    std::cout << "[DEBUG] HandleEnemyWithFireball called!" << std::endl;
    if (AtoB == COLLISION_TYPE_NONE) {
        std::cout << "No collision between Enemy and Fireball" << std::endl;
        return;
    }

    if (dynamic_cast<BuzzyBeetle*>(enemy)) {
        std::cout << "BuzzyBeetle is immune to fireball!" << std::endl;
        return;
    }

    // Các quái khác sẽ chết khi trúng fireball
    //std::cout << "Enemies size before: " << enemies.size() << std::endl;
    // enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
    // delete enemy;
    // enemy = nullptr;
    enemy->SetState(OBJECT_STATE_DEAD);
    std::cout << "Enemy dies by fireball" << std::endl;
}