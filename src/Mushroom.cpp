#include "Mushroom.h"
#include "GameWorld.h"
#include "ResrcManager.h"
#include "Tile.h"
#include "GameClock.h"

Mushroom::Mushroom(Vector2 pos)
    : Item(pos, {32, 32}, {40, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      applyGravity(false),
      isMoving(false) {
    state = OBJECT_STATE_ACTIVE;
    sprite = &ResrcManager::GetInstance().getTexture("Mushroom");
    // StartMoving(); 
}

void Mushroom::StartMoving() {
    if (!isMoving) {
        vel.x = 40.0f;           
        vel.y = 0.0f;
        applyGravity = true;
        isMoving = true;
    }
}

void Mushroom::updateMario(Mario& mario) {
    mario.changeToBig();
    SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void Mushroom::playCollisionSound() {
    // Optional
}

void Mushroom::Update() {
    float dt = GetFrameTime();

    if (state != OBJECT_STATE_ACTIVE)
        return;

    if (applyGravity)
        vel.y += GameWorld::GetGravity() * dt;

    if (state == OBJECT_STATE_ON_GROUND) {
        vel.y = 0;

        if (vel.x > maxSpeedX) vel.x = maxSpeedX;
        if (vel.x < -maxSpeedX) vel.x = -maxSpeedX;
    }

    if (vel.y > 0)
        state = OBJECT_STATE_FALLING;

    pos.x += vel.x * GameClock::getInstance().FIXED_TIME_STEP;
    pos.y += vel.y * dt;

    UpdateCollisionProbes();
}

void Mushroom::draw() {
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (sprite)
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
}
