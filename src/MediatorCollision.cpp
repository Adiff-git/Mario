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

void MediatorCollision::HandleItemWithTile(Item *&item, Tile *&tile, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;
    switch (AtoB)
    {
        case COLLISION_TYPE_SOUTH:
        {
            // Đặt item lên trên tile và cho di chuyển sang phải
            
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
    // item with tile
    else if ((isAitem && isBtile) || (isAtile && isBitem)){
        CollisionType AtoB = isAitem ? isAitem->checkCollisionType(*isBtile) : isBitem->checkCollisionType(*isAtile);
        if (isAitem)
            HandleItemWithTile(isAitem, isBtile, AtoB);
        else
            HandleItemWithTile(isBitem, isAtile, AtoB);
    }

   else if ((isAmario && isBitem) || (isBmario && isAitem))
    {
       Mario *mario = isAmario ? isAmario : isBmario;
       Item *item = isAitem ? isAitem : isBitem;

       if (item->checkCollision(*mario) == COLLISION_TYPE_COLLIDED)
       {
           item->updateMario(*mario);
           item->playCollisionSound(); // sound
       }
    }

}