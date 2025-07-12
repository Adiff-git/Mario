#include "OneUpMushroom.h"
#include "GameWorld.h"
#include "GameClock.h"
#include "Mario.h"
#include "ResrcManager.h"

OneUpMushroom::OneUpMushroom(Vector2 pos)
    : Item(pos, {32, 32}, {0, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      applyGravity(false),
      isMoving(false)
{
    state = OBJECT_STATE_ACTIVE;
    sprite = &ResrcManager::GetInstance().getTexture("1UpMushroom");

    StartMoving(); // Bắt đầu di chuyển ngay
}

void OneUpMushroom::StartMoving() {
    if (!isMoving) {
        vel.x = 40.0f;   // tương tự Mushroom
        vel.y = 0.0f;
        applyGravity = true;
        isMoving = true;
    }
}

void OneUpMushroom::Update() {
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

void OneUpMushroom::updateMario(Mario& mario)
{
    if (state == OBJECT_STATE_TO_BE_REMOVED) return;

    mario.SetLives(mario.GetLives() + 1);      // Thêm 1 mạng
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}


void OneUpMushroom::playCollisionSound() {
    // Optional
}

void OneUpMushroom::draw() {
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (sprite)
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
}
