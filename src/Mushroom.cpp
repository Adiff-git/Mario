#include "Mushroom.h"
#include "GameWorld.h"
#include "GameClock.h"
#include "Character.h"
#include "ResrcManager.h"

Mushroom::Mushroom(Vector2 pos)
    : Item(pos, {32, 32}, {40, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      applyGravity(true),
      isMoving(true),
      blinking(false), blinkingAcum(0), blinkingTime(0.1f), doBlink(false), blinkingAcumTotal(0)
{
    state = OBJECT_STATE_ACTIVE;
    sprite = &ResrcManager::GetInstance().getTexture("Mushroom");
}

void Mushroom::updateMario(Character& mario)
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || blinking)
        return;

    mario.changeToBig(); 
    blinking = true;     
    blinkingAcum = 0;
    blinkingAcumTotal = 0;
}

void Mushroom::playCollisionSound()
{

}

void Mushroom::Update()
{
    const float dt = 1.0f / 60.0f;
    float fixedDt = GameClock::GetInstance().FIXED_TIME_STEP;

    if (blinking) {
        blinkingAcum += GameClock::GetInstance().FIXED_TIME_STEP;
        blinkingAcumTotal += GameClock::GetInstance().FIXED_TIME_STEP;
        
        if (blinkingAcum >= blinkingTime) {
            doBlink = !doBlink;
            blinkingAcum = 0;
        }
        
        if (blinkingAcumTotal >= 0.8f) {
            this->SetState(OBJECT_STATE_TO_BE_REMOVED);
            return;
        }
    }

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
            vel.y = -200.f; 
            state = OBJECT_STATE_ACTIVE;
        }
    }
}

void Mushroom::Draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (blinking && doBlink) return;

    if (sprite)
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
}