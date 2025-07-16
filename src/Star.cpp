#include "GameWorld.h"
#include "Mario.h"
#include "raylib.h"
#include "ResrcManager.h"
#include "Star.h"
#include "GameClock.h"

Star::Star(Vector2 pos)
    : Item(pos, {32, 32}, {100, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      applyGravity(true), isMoving(true)
{
    state = OBJECT_STATE_ACTIVE;
    sprite = &ResrcManager::GetInstance().getTexture("star");
}

void Star::updateMario(Mario& mario)
{
    mario.setInvincible(true);
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void Star::playCollisionSound()
{
    // Optional
}

void Star::Update()
{
    float dt = GetFrameTime();
    float fixedDt = GameClock::GetInstance().FIXED_TIME_STEP;

    if (state == OBJECT_STATE_ACTIVE || state == OBJECT_STATE_FALLING)
    {
        
        vel.y += GameWorld::GetGravity() * dt;

        
        if (vel.y > 0)
        {
            state = OBJECT_STATE_FALLING;
        }
        else if (vel.y < 0 && state != OBJECT_STATE_ON_GROUND)
        {
            state = OBJECT_STATE_ACTIVE; 
        }

        
        pos.x += vel.x * fixedDt;
        pos.y += vel.y * dt;

        
        UpdateCollisionProbes();

        
        if (state == OBJECT_STATE_ON_GROUND)
        {
            vel.y = -250.f; // Bật lên
            state = OBJECT_STATE_ACTIVE; 
        }
    }
}


void Star::Draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (sprite)
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
}