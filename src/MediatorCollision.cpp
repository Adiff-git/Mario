#include "MediatorCollision.h"
#include "Enemy.h"
#include "Goomba.h"
#include "GreenKoopa.h"
#include "BuzzyBeetle.h"
#include "Rex.h"
#include "FlyingGoomba.h"
#include "PiranhaPlant.h"
#include "BossFireball.h"
#include "Boss.h"
#include <iostream>
#include <algorithm>

// Initialize static member
int MediatorCollision::marioFireballHits = 0;
void MediatorCollision::HandleMarioWithTile(Mario *&mario, Tile *&tile, CollisionType AtoB)
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
        item->SetVel(Vector2{item->GetVel().x, 0}); // Cho phép item tiếp tục di chuyển
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

void MediatorCollision::HandleCollision(Object *ObjectA, Object *ObjectB)
{
    Mario *marioA = dynamic_cast<Mario *>(ObjectA);
    Mario *marioB = dynamic_cast<Mario *>(ObjectB);

    Fireball *fireballA = dynamic_cast<Fireball *>(ObjectA);
    Fireball *fireballB = dynamic_cast<Fireball *>(ObjectB);

    BossFireball *bossFireballA = dynamic_cast<BossFireball *>(ObjectA);
    BossFireball *bossFireballB = dynamic_cast<BossFireball *>(ObjectB);

    Tile *tileA = dynamic_cast<Tile *>(ObjectA);
    Tile *tileB = dynamic_cast<Tile *>(ObjectB);

    Enemy *enemyA = dynamic_cast<Enemy *>(ObjectA);
    Enemy *enemyB = dynamic_cast<Enemy *>(ObjectB);

    Boss *bossA = dynamic_cast<Boss *>(ObjectA);
    Boss *bossB = dynamic_cast<Boss *>(ObjectB);

    Item *itemA = dynamic_cast<Item *>(ObjectA);
    Item *itemB = dynamic_cast<Item *>(ObjectB);

    Block *blockA = dynamic_cast<Block *>(ObjectA);
    Block *blockB = dynamic_cast<Block *>(ObjectB);

    // Boss vs Fireball (HIGHEST PRIORITY - check this FIRST before Enemy vs Fireball)
    if ((bossA && fireballB) || (bossB && fireballA))
    {
        std::cout << "[DEBUG] ===== BOSS vs FIREBALL DETECTED =====" << std::endl;
        Boss *boss = bossA ? bossA : bossB;
        Fireball *fireball = fireballA ? fireballA : fireballB;
        
        std::cout << "[DEBUG] Boss pointer: " << boss << std::endl;
        std::cout << "[DEBUG] Fireball pointer: " << fireball << std::endl;
        std::cout << "[DEBUG] Boss at (" << boss->GetPos().x << "," << boss->GetPos().y
                  << ") size (" << boss->GetSize().x << "," << boss->GetSize().y << ")" << std::endl;
        std::cout << "[DEBUG] Fireball at (" << fireball->GetPos().x << "," << fireball->GetPos().y 
                  << ") size (" << fireball->GetSize().x << "," << fireball->GetSize().y << ")" << std::endl;
        
        CollisionType type = boss->checkCollisionType(*fireball);
        std::cout << "[DEBUG] Boss->checkCollisionType(*fireball) returned: " << type << std::endl;
        
        HandleBossWithFireball(boss, fireball, type);
        return; // Early return to prevent other collision checks
    }

    // Boss vs Tile
    else if ((bossA && tileB) || (bossB && tileA))
    {
        Boss *boss = bossA ? bossA : bossB;
        Tile *tile = tileA ? tileA : tileB;
        CollisionType type = boss->checkCollisionType(*tile);
        HandleBossWithTile(boss, tile, type);
    }

    // Mario vs Tile
    else if ((marioA && tileB) || (marioB && tileA))
    {
        Mario *mario = marioA ? marioA : marioB;
        Tile *tile = tileA ? tileA : tileB;
        CollisionType type = mario->checkCollisionType(*tile);
        HandleMarioWithTile(mario, tile, type);
    }

    // Fireball vs Tile
    else if ((fireballA && tileB) || (fireballB && tileA))
    {
        Fireball *fireball = fireballA ? fireballA : fireballB;
        Tile *tile = tileA ? tileA : tileB;
        CollisionType type = fireball->checkCollisionType(*tile);
        HandleFireballWithTile(fireball, tile, type);
    }

    // Item vs Tile
    else if ((itemA && tileB) || (itemB && tileA))
    {
        Item *item = itemA ? itemA : itemB;
        Tile *tile = tileA ? tileA : tileB;
        CollisionType type = item->checkCollisionType(*tile);
        HandleItemWithTile(item, tile, type);
    }

    // Mario vs Item
    else if ((marioA && itemB) || (marioB && itemA))
    {
        Mario *mario = marioA ? marioA : marioB;
        Item *item = itemA ? itemA : itemB;

        if (item->canBeCollected() && item->checkCollision(*mario) == COLLISION_TYPE_COLLIDED)
        {
            item->updateMario(*mario);
            item->playCollisionSound();
        }
    }

    // Mario vs Boss (MUST be before Mario vs Enemy since Boss inherits from Enemy)
    else if ((marioA && bossB) || (marioB && bossA))
    {
        Mario *mario = marioA ? marioA : marioB;
        Boss *boss = bossA ? bossA : bossB;
        CollisionType type = mario->checkCollisionType(*boss);
        HandleMarioWithBoss(mario, boss, type);
    }

    // Mario vs Enemy
    else if ((marioA && enemyB) || (marioB && enemyA))
    {
        Mario *mario = marioA ? marioA : marioB;
        Enemy *enemy = enemyA ? enemyA : enemyB;
        CollisionType type = mario->checkCollisionType(*enemy);
        HandleMarioWithEnemy(mario, enemy, type);
    }

    // Enemy vs Tile
    else if ((enemyA && tileB) || (enemyB && tileA))
    {
        Enemy *enemy = enemyA ? enemyA : enemyB;
        Tile *tile = tileA ? tileA : tileB;
        CollisionType type = enemy->checkCollisionType(*tile);
        HandleEnemyWithTile(enemy, tile, type);
    }

    // Enemy vs Fireball (but skip if it's actually a Boss)
    else if ((enemyA && fireballB) || (enemyB && fireballA))
    {
        Enemy *enemy = enemyA ? enemyA : enemyB;
        Fireball *fireball = fireballA ? fireballA : fireballB;

        // Double-check: if this Enemy is actually a Boss, skip it
        if (!dynamic_cast<Boss *>(enemy))
        {
            CollisionType type = enemy->checkCollisionType(*fireball);
            HandleEnemyWithFireball(enemy, fireball, type);
        }
    }

    // Mario vs block
    else if ((marioA && blockB) || (blockA && marioB))
    {
        Block *block = blockA ? blockA : blockB;
        Mario *mario = marioA ? marioA : marioB;
        CollisionType type = mario->checkCollisionType(*block);
        HandleMarioWithBlock(mario, block, type);
    }

    // Mario vs BossFireball
    else if ((marioA && bossFireballB) || (marioB && bossFireballA))
    {
        Mario *mario = marioA ? marioA : marioB;
        BossFireball *bossFireball = bossFireballA ? bossFireballA : bossFireballB;
        CollisionType type = mario->checkCollisionType(*bossFireball);
        HandleMarioWithBossFireball(mario, bossFireball, type);
    }
}

void MediatorCollision ::HandleMarioWithBlock(Mario *&mario, Block *&block, CollisionType type)
{
    if (type == COLLISION_TYPE_NONE)
        return;
    switch (type)
    {
    case COLLISION_TYPE_SOUTH:
    {
        switch (block->GetBlockType())
        {
        case BLOCK_QUESTION:
            mario->SetPos(Vector2{mario->GetPos().x, block->GetPos().y - mario->GetSize().y});
            mario->SetState(OBJECT_STATE_ON_GROUND);
            mario->SetVel(Vector2{mario->GetVel().x, 0});
            break;
        case BLOCK_CLOUD:
            if (mario->GetState() == OBJECT_STATE_FALLING)
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
            if (!block->isHit())
            {
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
    case COLLISION_TYPE_NORTH:
    {
        switch (block->GetBlockType())
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
            if (!block->isHit())
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
    case COLLISION_TYPE_EAST:
    {
        switch (block->GetBlockType())
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
            if (!block->isHit())
            {
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
    case COLLISION_TYPE_WEST:
    {
        switch (block->GetBlockType())
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
            if (!block->isHit())
            {
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

void MediatorCollision::HandleBossWithTile(Boss *&boss, Tile *&tile, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;
        
    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        // Boss hits ground - position boss on top of tile
        boss->SetPos(Vector2{boss->GetPos().x, tile->GetPos().y - boss->GetSize().y});
        boss->SetVel(Vector2{boss->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_NORTH:
    {
        // Boss hits ceiling - position boss below tile
        boss->SetPos(Vector2{boss->GetPos().x, tile->GetPos().y + tile->GetSize().y});
        boss->SetVel(Vector2{boss->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_EAST:
    {
        // Boss hits wall from left - position boss to left of tile and reverse direction
        boss->SetPos(Vector2{tile->GetPos().x - boss->GetSize().x, boss->GetPos().y});
        boss->SetVel(Vector2{-boss->GetVel().x, boss->GetVel().y});
        boss->SetDirection(DIRECTION_LEFT);
        break;
    }
    case COLLISION_TYPE_WEST:
    {
        // Boss hits wall from right - position boss to right of tile and reverse direction
        boss->SetPos(Vector2{tile->GetPos().x + tile->GetSize().x, boss->GetPos().y});
        boss->SetVel(Vector2{-boss->GetVel().x, boss->GetVel().y});
        boss->SetDirection(DIRECTION_RIGHT);
        break;
    }
    }
}

void MediatorCollision::HandleMarioWithEnemy(Mario *&mario, Enemy *&enemy, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
    {
        std::cout << "No collision between Mario and Enemy" << std::endl;
        return;
    }

    // Check if this enemy is actually a Boss - Boss should be handled by HandleMarioWithBoss
    if (dynamic_cast<Boss *>(enemy))
    {
        std::cout << "This is a Boss! Should be handled by HandleMarioWithBoss, not HandleMarioWithEnemy!" << std::endl;
        return; // Don't process Boss here, let HandleMarioWithBoss handle it
    }

    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        mario->SetVel(Vector2{mario->GetVel().x, -300.0f});

        Rex *rex = dynamic_cast<Rex *>(enemy);
        if (rex)
        {
            rex->OnHit();
            if (rex->GetHitCount() >= 2)
            {
                if (!rex->IsBlinking())
                {
                    rex->StartBlinking(2.0f, 0.1f);
                }
            }
        }
        else
        {
            if (!enemy->IsBlinking())
            {
                enemy->StartBlinking(2.0f, 0.1f);
                enemy->SetHitByFireball(true);
            }

            RedKoopa *redKoopa = dynamic_cast<RedKoopa *>(enemy);
            if (redKoopa)
            {
                float marioX = mario->GetPos().x;
                float koopaX = redKoopa->GetPos().x;
                bool fromLeft = (marioX < koopaX);
                redKoopa->OnHit(fromLeft);
            }
        }
        break;
    }
    //====================================================
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
    std::cout << "[DEBUG] HandleEnemyWithFireball called!" << std::endl;
    if (AtoB == COLLISION_TYPE_NONE)
    {
        std::cout << "No collision between Enemy and Fireball" << std::endl;
        return;
    }

    // Check if this enemy is actually a Boss - Boss should be handled by HandleBossWithFireball
    if (dynamic_cast<Boss *>(enemy))
    {
        std::cout << "This is a Boss! Should be handled by HandleBossWithFireball, not HandleEnemyWithFireball!" << std::endl;
        return; // Don't process Boss here, let HandleBossWithFireball handle it
    }

    if (dynamic_cast<BuzzyBeetle *>(enemy))
    {
        std::cout << "BuzzyBeetle is immune to fireball!" << std::endl;
        return;
    }

    // Các quái khác sẽ chết khi trúng fireball
    // std::cout << "Enemies size before: " << enemies.size() << std::endl;
    // enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
    // delete enemy;
    // enemy = nullptr;
    enemy->SetState(OBJECT_STATE_DEAD);
    std::cout << "Enemy dies by fireball" << std::endl;
}

void MediatorCollision::HandleMarioWithBossFireball(Mario *&mario, BossFireball *&bossFireball, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;

    std::cout << "BossFireball collision detected! Type: " << AtoB << std::endl;

    // Handle any collision type (similar to Mario vs Enemy)
    if (AtoB != COLLISION_TYPE_NONE)
    {
        // Increment fireball hit counter first
        marioFireballHits++;
        std::cout << "Mario hit by Boss Fireball! Hits: " << marioFireballHits << "/" << maxFireballHits << std::endl;

        // Let Mario's BeHit() handle damage and invincibility normally
        mario->BeHit(); // This will handle state changes, invincibility, damage, etc.
        std::cout << "Mario hit by BossFireball - BeHit() called" << std::endl;

        // Check if this is the final hit for death (after BeHit processing)
        if (marioFireballHits >= maxFireballHits)
        {
            std::cout << "Mario reaches " << maxFireballHits << " Boss Fireball hits - forcing death!" << std::endl;
            mario->Die(); // Force death after max hits
            // Reset counter after death
            marioFireballHits = 0;
        }

        // Mark BossFireball as consumed/destroyed
        bossFireball->SetState(OBJECT_STATE_DEAD);
        std::cout << "BossFireball destroyed after hitting Mario" << std::endl;
    }
}

void MediatorCollision::HandleMarioWithBoss(Mario *&mario, Boss *&boss, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
    {
        std::cout << "No collision between Mario and Boss" << std::endl;
        return;
    }

    std::cout << "Mario vs Boss collision detected! Type: " << AtoB << std::endl;

    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        // Mario jumps on Boss - Boss takes damage like fireball hit
        std::cout << "Mario jumped on Boss - Boss takes damage from stomping!" << std::endl;
        mario->SetVel(Vector2{mario->GetVel().x, -300.0f}); // Mario bounces
        
        // Boss takes damage similar to fireball hit
        std::cout << "Boss hit by Mario's stomp! Hit count before: " << boss->GetHitCount() << std::endl;
        boss->OnHitByFireball(); // Use same damage method as fireball
        std::cout << "Boss hit count after: " << boss->GetHitCount() << "/" << 10 << std::endl;
        
        // Check if Boss should die after 10 hits
        if (boss->GetHitCount() >= 10)
        {
            std::cout << "Boss defeated after 10 hits (including stomp)!" << std::endl;
        }
        break;
    }
    case COLLISION_TYPE_EAST:
    case COLLISION_TYPE_WEST:
    case COLLISION_TYPE_NORTH:
    {
        // Boss hurts Mario on side/top collision
        std::cout << "Boss damages Mario on side collision!" << std::endl;
        mario->BeHit();
        break;
    }
    }
}

void MediatorCollision::HandleBossWithFireball(Boss *&boss, Fireball *&fireball, CollisionType AtoB)
{
    std::cout << "[DEBUG] ===== BOSS FIREBALL COLLISION START =====" << std::endl;
    std::cout << "[DEBUG] HandleBossWithFireball called!" << std::endl;
    std::cout << "[DEBUG] Boss position: (" << boss->GetPos().x << ", " << boss->GetPos().y << ")" << std::endl;
    std::cout << "[DEBUG] Boss size: (" << boss->GetSize().x << ", " << boss->GetSize().y << ")" << std::endl;
    std::cout << "[DEBUG] Fireball position: (" << fireball->GetPos().x << ", " << fireball->GetPos().y << ")" << std::endl;
    std::cout << "[DEBUG] Fireball size: (" << fireball->GetSize().x << ", " << fireball->GetSize().y << ")" << std::endl;
    std::cout << "[DEBUG] Collision type: " << AtoB << std::endl;

    if (AtoB == COLLISION_TYPE_NONE)
    {
        std::cout << "[DEBUG] No collision detected, returning" << std::endl;
        std::cout << "[DEBUG] ===== BOSS FIREBALL COLLISION END (NO COLLISION) =====" << std::endl;
        return;
    }

    std::cout << "Boss hit by Mario's Fireball! Hit count before: " << boss->GetHitCount() << std::endl;

    // Boss takes damage from Mario's fireball
    boss->OnHitByFireball();

    std::cout << "Boss hit count after: " << boss->GetHitCount() << "/" << 10 << std::endl;

    // Check if Boss should die after 10 hits (Boss handles this internally in OnHitByFireball)
    if (boss->GetHitCount() >= 10)
    {
        std::cout << "Boss defeated after 10 fireball hits!" << std::endl;
        // Boss already handles setting OBJECT_STATE_DEAD in OnHitByFireball method
    }

    // Destroy the fireball after hitting the boss
    fireball->SetState(OBJECT_STATE_DEAD);
    std::cout << "Fireball destroyed after hitting Boss" << std::endl;
    std::cout << "[DEBUG] ===== BOSS FIREBALL COLLISION END (SUCCESS) =====" << std::endl;
}
