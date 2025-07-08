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
    Block* isAquestionBlock = dynamic_cast<QuestionBlock*>(ObjectA);
    Block* isBquestionBlock = dynamic_cast<QuestionBlock*>(ObjectB);
    CloudBlock* isAcloudBlock = dynamic_cast<CloudBlock*>(ObjectA);
    CloudBlock* isBcloudBlock = dynamic_cast<CloudBlock*>(ObjectB);
    WoodBlock* isAwoodBlock = dynamic_cast<WoodBlock*>(ObjectA);
    WoodBlock* isBwoodBlock = dynamic_cast<WoodBlock*>(ObjectB);
    if ((isAmario && isBtile) || (isBmario && isAtile))
    {
        CollisionType AtoB = isAmario ? isAmario->checkCollisionType(*isBtile) : isBmario->checkCollisionType(*isAtile);
        if (isAmario)
            HandleMarioWithTile(isAmario, isBtile, AtoB);
        else
            HandleMarioWithTile(isBmario, isAtile, AtoB);
    }
    else if ((isAfireball && isBtile) || (isBfireball && isAtile))
    {
        CollisionType AtoB = isAfireball ? isAfireball->checkCollisionType(*isBtile) : isBfireball->checkCollisionType(*isAtile);
        if (isAfireball)
            HandleFireballWithTile(isAfireball, isBtile, AtoB);
        else
            HandleFireballWithTile(isBfireball, isAtile, AtoB);
    }
    else if (isAmario && isBquestionBlock || isBmario && isAquestionBlock)
    {
        CollisionType AtoB = isAmario ? isAmario->checkCollisionType(*isBquestionBlock) : isBmario->checkCollisionType(*isAquestionBlock);

        if (isAmario)
            HandleMarioWithQuestionBlock(isAmario, isBquestionBlock, AtoB);
        else
            HandleMarioWithQuestionBlock(isBmario, isAquestionBlock, AtoB);
    }
    else if ((isAmario && isBcloudBlock) || (isBmario && isAcloudBlock))
    {
        CollisionType AtoB = isAmario ? isAmario->checkCollisionType(*isBcloudBlock) : isBmario->checkCollisionType(*isAcloudBlock);
        if (isAmario)
            HandleMarioWithCloudBlock(isAmario, isBcloudBlock, AtoB);
        else
            HandleMarioWithCloudBlock(isBmario, isAcloudBlock, AtoB);
    }
    else if ((isAmario && isBwoodBlock) || (isBmario && isAwoodBlock))
    {
        CollisionType AtoB = isAmario ? isAmario->checkCollisionType(*isBwoodBlock) : isBmario->checkCollisionType(*isAwoodBlock);
        if (isAmario)
            HandleMarioWithWoodBlock(isAmario, isBwoodBlock, AtoB);
        else
            HandleMarioWithWoodBlock(isBmario, isAwoodBlock, AtoB);
    }
    // else if (isAfireball && isBblock || isBfireball && isAblock)
    // {
    //     CollisionType AtoB = isAfireball ? isAfireball->checkCollisionType(*isBblock) : isBfireball->checkCollisionType(*isAblock);

    //     if (isAfireball)
    //         HandleFireballWithBlock(isAfireball, isBblock, AtoB);
    //     else
    //         HandleFireballWithBlock(isBfireball, isAblock, AtoB);
    // }
}
void MediatorCollision::HandleMarioWithQuestionBlock(Mario* &mario, Block *&questionBlock, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;

    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
    {
        mario->SetPos(Vector2{mario->GetPos().x, questionBlock->GetPos().y - mario->GetSize().y});
        mario->SetState(OBJECT_STATE_ON_GROUND);
        mario->SetVel(Vector2{mario->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_NORTH:
    {
        mario->SetPos(Vector2{mario->GetPos().x, questionBlock->GetPos().y + questionBlock->GetSize().y});
        mario->SetVel(Vector2{mario->GetVel().x, 0});
        break;
    }
    case COLLISION_TYPE_EAST:
    {
        mario->SetPos(Vector2{questionBlock->GetPos().x - mario->GetSize().x, mario->GetPos().y});
        mario->SetVel(Vector2{0, mario->GetVel().y});
        break;
    }
    case COLLISION_TYPE_WEST:
    {
        mario->SetPos(Vector2{questionBlock->GetPos().x + questionBlock->GetSize().x, mario->GetPos().y});
        mario->SetVel(Vector2{0, mario->GetVel().y});
        break;
    }
    default:
        break;
    }
}

void MediatorCollision::HandleMarioWithCloudBlock(Mario* &mario, CloudBlock *&cloudBlock, CollisionType AtoB)
{
    if (AtoB == COLLISION_TYPE_NONE)
        return;
    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
        if(mario->GetState() == OBJECT_STATE_FALLING)
        mario->SetPos(Vector2{mario->GetPos().x, cloudBlock->GetPos().y - mario->GetSize().y});
        mario->SetState(OBJECT_STATE_ON_GROUND);
        mario->SetVel(Vector2{mario->GetVel().x, 0});
        break;
    case COLLISION_TYPE_NORTH:
        // mario->SetPos(Vector2{mario->GetPos().x, cloudBlock->GetPos().y + cloudBlock->GetSize().y});
        // mario->SetVel(Vector2{mario->GetVel().x, 0});
        break;
    case COLLISION_TYPE_EAST:
        // mario->SetPos(Vector2{cloudBlock->GetPos().x - mario->GetSize().x, mario->GetPos().y});
        // mario->SetVel(Vector2{0, mario->GetVel().y});
        break;
    case COLLISION_TYPE_WEST:
        // mario->SetPos(Vector2{cloudBlock->GetPos().x + cloudBlock->GetSize().x, mario->GetPos().y});
        // mario->SetVel(Vector2{0, mario->GetVel().y});
        break;
    default:
        break;
    }
}

void MediatorCollision::HandleMarioWithWoodBlock(Mario* &mario, WoodBlock *&woodBlock, CollisionType AtoB){
    if (AtoB == COLLISION_TYPE_NONE)
        return;
    switch (AtoB)
    {
    case COLLISION_TYPE_SOUTH:
        mario->SetPos(Vector2{mario->GetPos().x, woodBlock->GetPos().y - mario->GetSize().y});
        mario->SetState(OBJECT_STATE_ON_GROUND);
        mario->SetVel(Vector2{mario->GetVel().x, 0});
        break;
    case COLLISION_TYPE_NORTH:
        mario->SetPos(Vector2{mario->GetPos().x, woodBlock->GetPos().y + woodBlock->GetSize().y});
        mario->SetVel(Vector2{mario->GetVel().x, 0});
        break;
    case COLLISION_TYPE_EAST:
        mario->SetPos(Vector2{woodBlock->GetPos().x - mario->GetSize().x, mario->GetPos().y});
        mario->SetVel(Vector2{0, mario->GetVel().y});
        break;
    case COLLISION_TYPE_WEST:
        mario->SetPos(Vector2{woodBlock->GetPos().x + woodBlock->GetSize().x, mario->GetPos().y});
        mario->SetVel(Vector2{0, mario->GetVel().y});
        break;
    default:
        break;
    }
}