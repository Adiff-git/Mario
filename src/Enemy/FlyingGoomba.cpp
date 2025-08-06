#include "../inc/Enemy/FlyingGoomba.h"
#include "../inc/World/GameClock.h"
#include "../inc/World/GameWorld.h"
#include "ResrcManager.h"

FlyingGoomba::FlyingGoomba(Vector2 pos)
    : Enemy(pos, Vector2{48, 32}, Vector2{0, 0}, BROWN, 0.2f, 0, DIRECTION_LEFT)
{
    sprite = &ResrcManager::GetInstance().getTexture("FlyingGoomba_Flap_Right");

    vel.x = -25.0f;  
    maxSpeedX = 25.0f;  
    textureIndex = 0;
    
    // Khởi tạo biến cho chuyển động qua lại
    startX = pos.x;
    baseY = pos.y;
    movingLeft = true;
}

void FlyingGoomba::UpdateStateAndPhysic()
{

    UpdateDyingState();
    
    if (state == OBJECT_STATE_DYING || state == OBJECT_STATE_DEAD) {
        UpdateDeathEffect();
        return;
    }
    const int updateThreshold = 15;
    updateCount++;

    if (updateCount >= updateThreshold)
    {
        float deltaTime = GetFrameTime();

        if (movingLeft) {
            vel.x = -maxSpeedX;
            direction = DIRECTION_LEFT;
            
            if (pos.x <= startX - moveDistance) {
                movingLeft = false; 
            }
        } else {
            vel.x = maxSpeedX;
            direction = DIRECTION_RIGHT;
            
            if (pos.x >= startX) {
                movingLeft = true; 
            }
        }

        pos.x += vel.x * deltaTime;

        flyingCycleTime += deltaTime;
        if (flyingCycleTime > flyingCycleDuration)
            flyingCycleTime -= flyingCycleDuration;

        float phase = (flyingCycleTime / flyingCycleDuration) * 2 * PI;
        pos.y = baseY + sin(phase) * flyingAmplitude;

        // std::cerr << updateCount << '\n';
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