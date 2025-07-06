#include "MediatorCollision.h"
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
void MediatorCollision::HandleCollision(Object *ObjectA, Object *ObjectB)
{
    Mario* isAmario = dynamic_cast<Mario*>(ObjectA);
    Mario* isBmario = dynamic_cast<Mario*>(ObjectB);
    Fireball* isAfireball = dynamic_cast<Fireball*>(ObjectA);
    Fireball* isBfireball = dynamic_cast<Fireball*>(ObjectB);
    Tile* isAtile = dynamic_cast<Tile*>(ObjectA);
    Tile* isBtile = dynamic_cast<Tile*>(ObjectB);
    Enemy* isAenemy = dynamic_cast<Enemy*>(ObjectA); 
    Enemy* isBenemy = dynamic_cast<Enemy*>(ObjectB); 
    if (isAmario && isBtile|| isBmario&& isAtile)
    {
        CollisionType AtoB = isAmario ? isAmario->checkCollisionType(*isBtile) : isBmario->checkCollisionType(*isAtile);
        if (isAmario)
            HandleMarioWithTile(isAmario, isBtile, AtoB);
        else
            HandleMarioWithTile(isBmario, isAtile, AtoB);
    }
    else if (isAfireball && isBtile || isBfireball && isAtile)
    {
        CollisionType AtoB = isAfireball ? isAfireball->checkCollisionType(*isBtile) : isBfireball->checkCollisionType(*isAtile);
        if (isAfireball)
            HandleFireballWithTile(isAfireball, isBtile, AtoB);
        else
            HandleFireballWithTile(isBfireball, isAtile, AtoB);
    }
    else if (isAenemy && isBtile) {
        CollisionType AtoB = isAenemy->checkCollisionType(*isBtile);
        HandleEnemyWithTile(isAenemy, isBtile, AtoB);
    } else if (isBenemy && isAtile) {
        CollisionType AtoB = isBenemy->checkCollisionType(*isAtile);
        HandleEnemyWithTile(isBenemy, isAtile, AtoB);
    }
    else if ((isAmario && isBenemy) || (isBmario && isAenemy)) {
    CollisionType AtoB = isAmario ? isAmario->checkCollisionType(*isBenemy) : isBmario->checkCollisionType(*isAenemy);
    if (isAmario)
        HandleMarioWithEnemy(isAmario, isBenemy, AtoB);
    else
        HandleMarioWithEnemy(isBmario, isAenemy, AtoB);}
        else if ((isAenemy && isBfireball) || (isBenemy && isAfireball)) {
        CollisionType AtoB = isAenemy ? isAenemy->checkCollisionType(*isBfireball) : isBenemy->checkCollisionType(*isAfireball);
        if (isAenemy)
            HandleEnemyWithFireball(isAenemy, isBfireball, AtoB);
        else
            HandleEnemyWithFireball(isBenemy, isAfireball, AtoB);
    }
}

void MediatorCollision::HandleEnemyWithTile(Enemy*& enemy, Tile* tile, CollisionType AtoB) {
    if (AtoB == COLLISION_TYPE_NONE) return;
    switch (AtoB) {
        case COLLISION_TYPE_SOUTH:{
            enemy->SetPos(Vector2{enemy->GetPos().x, tile->GetPos().y - enemy->GetSize().y});
            enemy->SetState(OBJECT_STATE_ON_GROUND);
            enemy->SetVel(Vector2{enemy->GetVel().x, 0});
            break;}
        case COLLISION_TYPE_EAST: {
            enemy->SetPos(Vector2{tile->GetPos().x - enemy->GetSize().x, enemy->GetPos().y});
            enemy->SetVel(Vector2{-enemy->GetVel().x, enemy->GetVel().y});
            enemy->SetDirection(DIRECTION_LEFT); // Cập nhật hướng sang trái
            break;
}
        case COLLISION_TYPE_WEST: {
            enemy->SetPos(Vector2{tile->GetPos().x + tile->GetSize().x, enemy->GetPos().y});
            enemy->SetVel(Vector2{-enemy->GetVel().x, enemy->GetVel().y});
            enemy->SetDirection(DIRECTION_RIGHT); // Cập nhật hướng sang phải
            break;
}
    }
}
void MediatorCollision::HandleMarioWithEnemy(Mario*& mario, Enemy*& enemy, CollisionType AtoB) {
    std::cout << "[DEBUG] HandleMarioWithEnemy called!" << std::endl;
    if (AtoB == COLLISION_TYPE_NONE) {
        std::cout << "No collision between Mario and Enemy" << std::endl;
        return;
    }

    std::cout << "Enemies size before: " << enemies.size() << std::endl;
    switch (AtoB) {
        case COLLISION_TYPE_SOUTH: {
            // Mario nhảy lên đầu Enemy -> Goomba chết
            std::cout << "Goomba dies" << std::endl;
            enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
            delete enemy;
            enemy = nullptr;
            mario->SetVel(Vector2{mario->GetVel().x, -20.0f}); // Mario bật lên
            break;
        }
        case COLLISION_TYPE_EAST:
        case COLLISION_TYPE_WEST:
        case COLLISION_TYPE_NORTH: {
            // Mario chạm ngang hoặc dưới -> Mario chết
            std::cout << "Mario dies ";
            break;
        }
    }
}
void MediatorCollision::HandleEnemyWithFireball(Enemy*& enemy, Fireball*& fireball, CollisionType AtoB) {
    std::cout << "[DEBUG] HandleEnemyWithFireball called!" << std::endl;
    if (AtoB == COLLISION_TYPE_NONE) {
        std::cout << "No collision between Enemy and Fireball" << std::endl;
        return;
    }

    std::cout << "Enemies size before: " << enemies.size() << std::endl;
    // Chỉ xóa enemy, KHÔNG xóa fireball
    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
    delete enemy;
    enemy = nullptr;
    std::cout << "Enemy dies by fireball" << std::endl;
}

std::vector<Enemy*>& MediatorCollision::GetEnemies() {
    return enemies;
}
MediatorCollision::MediatorCollision() {
    // Thêm Goomba ngay khi khởi tạo
    Enemy* goomba = new Goomba(Vector2{600, 100}); // Vị trí mặc định
    enemies.push_back(goomba);
}
