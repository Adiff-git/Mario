#include "Mushroom.h"
#include "GameWorld.h"
#include "ResrcManager.h"
#include "GameClock.h"

Mushroom::Mushroom(Vector2 pos)
    : Item(pos, {32, 32}, {40, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      applyGravity(false), isMoving(false), maxSpeedX(40.0f)
{
    state = OBJECT_STATE_ACTIVE;
    sprite = &ResrcManager::GetInstance().getTexture("Mushroom");
    StartMoving(); // Start moving immediately when spawned
}

void Mushroom::StartMoving() {
    if (!isMoving) {
        vel.x = 40.0f;
        vel.y = 0.0f;
        applyGravity = true;
        isMoving = true;
    }
}

void Mushroom::Update() {
    float dt = GetFrameTime();
    float fixedDt = GameClock::GetInstance().FIXED_TIME_STEP;

    if (state != OBJECT_STATE_ACTIVE)
        return;

    // Áp dụng trọng lực nếu có
    if (applyGravity) {
        vel.y += GameWorld::GetGravity() * dt;
    }

    // Nếu đang trên mặt đất thì giữ yên trục y và giới hạn vận tốc x
    if (state == OBJECT_STATE_ON_GROUND) {
        vel.y = 0;

        if (vel.x > maxSpeedX) vel.x = maxSpeedX;
        if (vel.x < -maxSpeedX) vel.x = -maxSpeedX;
    }

    // Cập nhật trạng thái rơi nếu cần
    if (vel.y > 0)
        state = OBJECT_STATE_FALLING;

    // Cập nhật vị trí
    pos.x += vel.x * fixedDt;
    pos.y += vel.y * dt;

    UpdateCollisionProbes();
}

void Mushroom::updateMario(Mario& mario) {
    mario.changeToBig();
    SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void Mushroom::playCollisionSound() {
    // Optional: Play sound here
}

void Mushroom::draw() {
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (sprite)
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
}
