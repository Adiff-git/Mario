#include "MediatorCollision.h"

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
    Item* isAitem = dynamic_cast<Item*>(ObjectA); 
    Item* isBitem = dynamic_cast<Item*>(ObjectB);

    // Mario <-> Tile
    if ((isAmario && isBtile) || (isBmario && isAtile))
    {
        CollisionType AtoB = isAmario ? isAmario->checkCollisionType(*isBtile) : isBmario->checkCollisionType(*isAtile);
        if (isAmario)
            HandleMarioWithTile(isAmario, isBtile, AtoB);
        else
            HandleMarioWithTile(isBmario, isAtile, AtoB);
    }
    // Fireball <-> Tile
    else if ((isAfireball && isBtile) || (isBfireball && isAtile))
    {
        CollisionType AtoB = isAfireball ? isAfireball->checkCollisionType(*isBtile) : isBfireball->checkCollisionType(*isAtile);
        if (isAfireball)
            HandleFireballWithTile(isAfireball, isBtile, AtoB);
        else
            HandleFireballWithTile(isBfireball, isAtile, AtoB);
    }
    // Item <-> Tile
    else if ((isAitem && isBtile) || (isBitem && isAtile))
    {
        Item* item = isAitem ? isAitem : isBitem;
        Tile* tile = isAtile ? isAtile : isBtile;
        CollisionType AtoB = item->checkCollisionType(*tile);
        HandleItemWithTile(item, tile, AtoB);
    }
    // Mario <-> Item
    else if ((isAmario && isBitem) || (isBmario && isAitem))
    {
        Mario* mario = isAmario ? isAmario : isBmario;
        Item* item = isAitem ? isAitem : isBitem;
        HandleMarioWithItem(mario, item);
    }
    // Item <-> Fireball
    else if ((isAitem && isBfireball) || (isBitem && isAfireball))
    {
        Item* item = isAitem ? isAitem : isBitem;
        Fireball* fireball = isAfireball ? isAfireball : isBfireball;
        HandleItemWithFireball(item, fireball);
    }
}

void MediatorCollision::HandleItemWithTile(Item*& item, Tile*& tile, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;

    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        item->SetPos(Vector2{item->GetPos().x, tile->GetPos().y - item->GetSize().y});
        // Always move right by default if stopped
        float speed = item->GetVel().x;
        if (speed == 0) speed = 40.0f;
        else speed = std::abs(speed); // always right
        item->SetVel(Vector2{speed, 0});
        item->SetDirection(DIRECTION_RIGHT);
        item->SetState(OBJECT_STATE_ACTIVE);
        break;
    }

    case COLLISION_TYPE_NORTH:
        item->SetPos(Vector2{item->GetPos().x, tile->GetPos().y + tile->GetSize().y});
        item->SetVel(Vector2{item->GetVel().x, 0});
        break;

    case COLLISION_TYPE_EAST:
    {
        item->SetPos(Vector2{tile->GetPos().x - item->GetSize().x, item->GetPos().y});
        float vxE = item->GetVel().x;
        if (vxE == 0) vxE = -40.0f; 
        else vxE = -std::abs(vxE);
        item->SetVel(Vector2{vxE, 0}); 
        item->SetDirection(DIRECTION_LEFT);
        item->SetState(OBJECT_STATE_ON_GROUND); 
        break;
    }
    case COLLISION_TYPE_WEST:
    {
        item->SetPos(Vector2{tile->GetPos().x + tile->GetSize().x, item->GetPos().y});
        float vxW = item->GetVel().x;
        if (vxW == 0) vxW = 40.0f; 
        else vxW = std::abs(vxW);
        item->SetVel(Vector2{vxW, 0}); 
        item->SetDirection(DIRECTION_RIGHT);
        item->SetState(OBJECT_STATE_ON_GROUND); 
        break;
    }

    default:
        break;
    }
}

void MediatorCollision::HandleMarioWithItem(Mario*& mario, Item*& item)
{
    if (item->checkCollision(*mario) == COLLISION_TYPE_COLLIDED)
    {
        item->updateMario(*mario);
        item->playCollisionSound();
    }
}

void MediatorCollision::HandleItemWithFireball(Item*& item, Fireball*& fireball)
{
    if (item->checkCollision(*fireball) == COLLISION_TYPE_COLLIDED)
    {
        item->onFireballHit(*fireball); // You may need to implement this in Item
        // Optionally, play a sound or handle fireball state
    }
}