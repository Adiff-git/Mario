#include "Mushroom.h"
#include "GameWorld.h"
#include "GameClock.h"
#include "Mario.h"
#include "ResrcManager.h"

Mushroom::Mushroom(Vector2 pos)
    : Item(pos, {32, 32}, {100, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      applyGravity(true),
      isMoving(true)
{
    state = OBJECT_STATE_ACTIVE;
    sprite = &ResrcManager::GetInstance().getTexture("Mushroom");
}

void Mushroom::updateMario(Mario& mario)
{
    if (state == OBJECT_STATE_TO_BE_REMOVED)
        return;

    mario.changeToBig(); // Mario biến to
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void Mushroom::playCollisionSound()
{
    // Optional: Add mushroom pickup sound
}

void Mushroom::Update()
{
    const float dt = 1.0f / 60.0f;
    float fixedDt = GameClock::GetInstance().FIXED_TIME_STEP;

    if (state == OBJECT_STATE_ACTIVE || state == OBJECT_STATE_FALLING)
    {
        vel.y += GameWorld::GetGravity() * dt;

        if (vel.y > 0)
            state = OBJECT_STATE_FALLING;
        else if (vel.y < 0 && state != OBJECT_STATE_ON_GROUND)
            state = OBJECT_STATE_ACTIVE;

        pos.x += vel.x * fixedDt;
        pos.y += vel.y * dt;

        UpdateCollisionProbes();

        if (state == OBJECT_STATE_ON_GROUND)
        {
            vel.y = -200.f; // Bật lên khi chạm đất
            state = OBJECT_STATE_ACTIVE;
        }
    }
}

void Mushroom::Draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (sprite)
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
}
