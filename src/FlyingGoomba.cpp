#include "FlyingGoomba.h"
#include "GameClock.h"
#include "GameWorld.h"
#include "ResrcManager.h"

FlyingGoomba::FlyingGoomba(Vector2 pos)
    : Enemy(pos, Vector2{48, 32}, Vector2{0, 0}, BROWN, 0.2f, 0, DIRECTION_RIGHT)
{
    sprite = &ResrcManager::GetInstance().getTexture("FlyingGoomba_Flap_Right");

    vel.x = 5.0f;
    maxSpeedX = 5.0f;
    textureIndex = 0;
}

void FlyingGoomba::UpdateStateAndPhysic()
{

    // Sprite animation
    const int updateThreshold = 15;
    updateCount++;

    if (updateCount >= updateThreshold)
    {
        float deltaTime = GetFrameTime();

        // Horizontal movement
        if (vel.x > 0)
            direction = DIRECTION_RIGHT;
        else if (vel.x < 0)
            direction = DIRECTION_LEFT;

        if (vel.x > maxSpeedX)
            vel.x = maxSpeedX;
        if (vel.x < -maxSpeedX)
            vel.x = -maxSpeedX;

        pos.x += vel.x * deltaTime; // Move forward

        // Add smooth vertical flap motion
        flyingCycleTime += deltaTime;
        if (flyingCycleTime > flyingCycleDuration)
            flyingCycleTime -= flyingCycleDuration;

        float phase = (flyingCycleTime / flyingCycleDuration) * 2 * PI;
        pos.y = baseY + sin(phase) * flyingAmplitude;

        std::cerr << updateCount << '\n';
        if (direction == DIRECTION_RIGHT)
        {
            sprite = &ResrcManager::GetInstance().getTexture(
                textureIndex == 0 ? "FlyingGoomba_Flap_Right" : "FlyingGoomba_WingDown_Right");
        }
        else
        {
            sprite = &ResrcManager::GetInstance().getTexture(
                textureIndex == 0 ? "FlyingGoomba_Flap_Center" : "FlyingGoomba_WingDown_Center");
        }

        textureIndex = 1 - textureIndex;
        updateCount = 0;
    }

    UpdateCollisionProbes();
}