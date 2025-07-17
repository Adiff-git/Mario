#include "OneUpMushroom.h"
#include "GameWorld.h"
#include "GameClock.h"
#include "Mario.h"
#include "ResrcManager.h"

OneUpMushroom::OneUpMushroom(Vector2 pos)
    : Item(pos, {32, 32}, {40, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      applyGravity(true),
      isMoving(true)
{
    state = OBJECT_STATE_ACTIVE;
    sprite = &ResrcManager::GetInstance().getTexture("1UpMushroom");
}

void OneUpMushroom::updateMario(Mario &mario)
{
    if (state == OBJECT_STATE_TO_BE_REMOVED)
        return;

    mario.SetLives(mario.GetLives() + 1); // Tăng mạng
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void OneUpMushroom::playCollisionSound()
{
    // Optional: Add sound effect
}

void OneUpMushroom::Update()
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
            vel.y = -200.f; // Bật lên nhẹ hơn star
            state = OBJECT_STATE_ACTIVE;
        }
    }
}

void OneUpMushroom::Draw()
{
    Update();
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (sprite)
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
}