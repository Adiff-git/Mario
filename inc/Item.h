#pragma once
#include "Object.h"
#include "Mario.h"
#include "raylib.h"

class Item : public Object {
protected:
    bool pauseGameOnHit;   

public:
    Item();
    Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color,
           float frameTime, int maxFrames, Direction facingDirection,
           int hitsToDie, int earnedPoints);
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
