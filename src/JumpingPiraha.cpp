#include "JumpingPiranha.h"
#include "ResrcManager.h"
#include "GameWorld.h"
#include "GameClock.h"

JumpingPiranhaPlant::JumpingPiranhaPlant(Vector2 pos)
    : Enemy(pos, {32, 48}, {0, 0}, GREEN, 0.0f, 0, DIRECTION_UP),
      animationTimer(0.0f),
      animationInterval(0.2f),
      textureIndex(0),
      directionAnim(1),
      updateCounter(0),
      updateThreshold(10)
{
    sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_0");
}

void JumpingPiranhaPlant::UpdateStateAndPhysic()
{
    const float deltaTime = GetFrameTime();

    updateCounter++;
    if (updateCounter >= updateThreshold)
    {
        textureIndex += directionAnim;
        if (textureIndex >= 3)
        {
            textureIndex = 3;
            directionAnim = -1;
        }
        else if (textureIndex <= 0)
        {
            textureIndex = 0;
            directionAnim = 1;
        }

        switch (textureIndex)
        {
        case 0:
            sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_0");
            break;
        case 1:
            sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_1");
            break;
        case 2:
            sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_2");
            break;
        case 3:
            sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_3");
            break;
        }

        updateCounter = 0;
    }

    if (GetState() != OBJECT_STATE_ON_GROUND)
    {
        vel.y += 9.81f * GameClock::getInstance().FIXED_TIME_STEP;
    }

    if (GetState() == OBJECT_STATE_ON_GROUND)
    {
        if (vel.x > maxSpeedX)
            vel.x = maxSpeedX;
        if (vel.x < -maxSpeedX)
            vel.x = -maxSpeedX;
        pos.x += vel.x * GameClock::getInstance().FIXED_TIME_STEP;
    }

    if (vel.y > 0)
        state = OBJECT_STATE_FALLING;
    if (state == OBJECT_STATE_ON_GROUND)
        vel.y = 0;

    vel.y += GameWorld::GetGravity() * deltaTime;

    // Cập nhật vị trí
    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;

    if (direction)
    {
        pos.y++;
    }
    else
    {
        pos.y--;
    }
    if (pos.y <= maxY)
    {
        direction = true;
    }
    if (pos.y >= minY)
    {
        direction = false;
    }

    // std::cerr << direction << '\n';

    UpdateCollisionProbes();
}