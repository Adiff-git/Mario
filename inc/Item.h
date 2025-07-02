#pragma once

#include "Object.h"
#include "Mario.h"
#include "raylib.h"

class Item : public Object {
protected:
    bool pauseGameOnHit;   

public:
    Item();
    virtual ~Item();

    void draw() override;

    //setter
    void SetSprite(Texture2D sprite);
    void SetState(ObjectState state);

    virtual void onSouthCollision(Mario& mario);
    virtual bool isPauseGameOnHit();
    virtual void updateMario(Mario& mario) = 0;
    virtual void playCollisionSound() = 0;
    virtual CollisionType checkCollision(const Object& other);
};
