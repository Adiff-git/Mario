#include "MediatorCollision.h"
#include <iostream>
#include <algorithm>
#include "EnemyManager.h"
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
                        enemies.erase(std::remove(enemies.begin(), enemies.end(), rex), enemies.end());
                        delete rex;
                        rex = nullptr;
                        std::cout << "Rex destroyed after 2 hits!" << std::endl;
                    }
                } else {
                    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
                    delete enemy;
                    enemy = nullptr;
                    mario->SetVel(Vector2{mario->GetVel().x, -300.0f});
                }
            }
            break;
        }
        //====================================================
        case COLLISION_TYPE_EAST:
        case COLLISION_TYPE_WEST:
        case COLLISION_TYPE_NORTH: {
            std::cout << "Mario dies ";
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

    // Kiểm tra nếu enemy là BuzzyBeetle
    if (dynamic_cast<BuzzyBeetle*>(enemy)) {
        std::cout << "BuzzyBeetle is immune to fireball!" << std::endl;
        return;
    }

    // Các quái khác sẽ chết khi trúng fireball
    std::cout << "Enemies size before: " << enemies.size() << std::endl;
    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
    delete enemy;
    enemy = nullptr;
    std::cout << "Enemy dies by fireball" << std::endl;
}


std::vector<Enemy*>& MediatorCollision::GetEnemies() {
    return enemies;
}
//=================================================
MediatorCollision::MediatorCollision() {
    
    //Enemy* goomba = new Goomba(Vector2{600, 100}); // Vị trí mặc định
    //enemies.push_back(goomba);
    //Enemy* greenKoopa = new GreenKoopa(Vector2{700, 100});
    //enemies.push_back(greenKoopa);
    //Enemy* yellowKoopa = new YellowKoopa(Vector2{800, 100});
    //enemies.push_back(yellowKoopa);
    //Enemy* redKoopa = new RedKoopa(Vector2{900, 100});
    //enemies.push_back(redKoopa);
    //Enemy* buzzyBeetle = new BuzzyBeetle(Vector2{1000, 100});
    //enemies.push_back(buzzyBeetle);
    //Enemy* bulletBill = new BulletBill(Vector2{1100, 100});
    //enemies.push_back(bulletBill);
    //Enemy* bobOmb = new Bob_omb(Vector2{1100, 100});
    //enemies.push_back(bobOmb);
    //Enemy* banzaiBill = new BanzaiBill(Vector2{1200, 100});
    //enemies.push_back(banzaiBill);
    //Enemy* rex = new Rex(Vector2{1300, 100});
    //enemies.push_back(rex);
    //Enemy* flyingGoomba = new FlyingGoomba(Vector2{1400, 100});
    //enemies.push_back(flyingGoomba);
    //Enemy* piranhaPlant = new PiranhaPlant(Vector2{1500, 100});
    //enemies.push_back(piranhaPlant);
    //Enemy* jumpingPiranhaPlant = new JumpingPiranhaPlant(Vector2{1600, 100});
    //enemies.push_back(jumpingPiranhaPlant);    

    
    //Không hiểu tại sao lại cần cái này nhưng không ghi thì bị lỗi animation của các quái khi spawn dưới đất
    //=================================================
}
