#include "Rex.h"
#include "GameWorld.h"
#include <cstdlib> // for rand()
#include <iostream> // Thêm để debug

Rex::Rex(Vector2 pos) 
    : Enemy(pos, Vector2{48, 64}, Vector2{5, 0}, GREEN, 0.3f, 0, DIRECTION_RIGHT) {
    sprite = &ResrcManager::GetInstance().getTexture("REX_0_RIGHT");
    jumpCooldown = 0;
    hitCount = 0;
    originalSize = GetSize();
    animationFrame = 0; // Khởi tạo animationFrame
}

void Rex::UpdateStateAndPhysic() {
    const float deltaTime = GetFrameTime();

    if (GetState() == OBJECT_STATE_ON_GROUND) {
        float newVelX = GetVel().x;
        if (newVelX > 0 && newVelX > maxSpeedX) newVelX = maxSpeedX;
        if (newVelX < 0 && newVelX < -maxSpeedX) newVelX = -maxSpeedX;
        SetVel(Vector2{newVelX, GetVel().y});
        SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::GetInstance().FIXED_TIME_STEP), GetPos().y});

    }

    if (GetVel().y > 0) {
        state = OBJECT_STATE_FALLING;
    }
    if (state == OBJECT_STATE_ON_GROUND) {
        SetVel(Vector2{GetVel().x, 0});
    }
    vel.y += GameWorld::GetGravity() * deltaTime;
    Object::UpdateStateAndPhysic();
   
    const int updateThreshold = 50;

    if (hitCount == 0) {
  
        if (fabs(GetVel().x) > 0.1f) {
            updateCount += 1;
            if (updateCount >= updateThreshold) {
                if (GetDirection() == DIRECTION_RIGHT) {
                    if (textureIndex == 0) {
                        sprite = &ResrcManager::GetInstance().getTexture("REX_0_RIGHT");
                        textureIndex = 1;
                    } else {
                        sprite = &ResrcManager::GetInstance().getTexture("REX_1_RIGHT");
                        textureIndex = 0;
                    }
                } else {
                    if (textureIndex == 0) {
                        sprite = &ResrcManager::GetInstance().getTexture("REX_0_LEFT");
                        textureIndex = 1;
                    } else {
                        sprite = &ResrcManager::GetInstance().getTexture("REX_1_LEFT");
                        textureIndex = 0;
                    }
                }
                updateCount = 0;
            }
        } else {
            if (GetDirection() == DIRECTION_RIGHT) {
                sprite = &ResrcManager::GetInstance().getTexture("REX_0_RIGHT");
            } else {
                sprite = &ResrcManager::GetInstance().getTexture("REX_0_LEFT");
            }
            textureIndex = 0;
            updateCount = 0;
        }
    } else if (hitCount >= 1) {

        if (fabs(GetVel().x) > 0.1f) {
            updateCount++;
            if (updateCount >= updateThreshold) {
                if (GetDirection() == DIRECTION_RIGHT) {
                    if (animationFrame == 0) {
                        sprite = &ResrcManager::GetInstance().getTexture("REX_3_RIGHT");
                        animationFrame = 1;
                    } else {
                        sprite = &ResrcManager::GetInstance().getTexture("REX_4_RIGHT");
                        animationFrame = 0;
                    }
                } else {
                    if (animationFrame == 0) {
                        sprite = &ResrcManager::GetInstance().getTexture("REX_3_LEFT");
                        animationFrame = 1;
                    } else {
                        sprite = &ResrcManager::GetInstance().getTexture("REX_4_LEFT");
                        animationFrame = 0;
                    }
                }
                updateCount = 0;
            }
        } else {
            if (GetDirection() == DIRECTION_RIGHT) {
                sprite = &ResrcManager::GetInstance().getTexture("REX_3_RIGHT");
            } else {
                sprite = &ResrcManager::GetInstance().getTexture("REX_4_LEFT");
            }
            animationFrame = 0;
            updateCount = 0;
        }
    }
}

void Rex::OnHit() {
    hitCount++;
    if (hitCount == 1) {
        sprite = &ResrcManager::GetInstance().getTexture("REX_3_RIGHT");
        SetVel(Vector2{GetVel().x/2, GetVel().y});
        SetSize(Vector2{originalSize.x, originalSize.y/2});
        SetPos(Vector2{GetPos().x, GetPos().y+46}); 
        UpdateCollisionProbes();
    }

}