#pragma once
#include "Object.h"
#include "Mario.h"
#include "Fireball.h"
#include "Tile.h"
#include "Item.h"

class MediatorCollision {
private:
    void HandleMarioWithTile(Mario*& mario, Tile*& tile, CollisionType AtoB);
    void HandleFireballWithTile(Fireball*& fireball, Tile*& tile, CollisionType AtoB);
    void HandleItemWithTile(Item*& item, Tile*& tile, CollisionType AtoB);
    void HandleMarioWithItem(Mario*& mario, Item*& item);
    void HandleItemWithFireball(Item*& item, Fireball*& fireball);

public:
    MediatorCollision() = default;
    ~MediatorCollision() = default;

    void HandleCollision(Object* ObjectA, Object* ObjectB);
};
