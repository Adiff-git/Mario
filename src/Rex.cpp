#include "Rex.h"
#include "GameWorld.h"
#include <cstdlib> 
#include <iostream> 

Rex::Rex(Vector2 pos) 
    : Enemy(pos, Vector2{48, 64}, Vector2{40, 0}, GREEN, 0.3f, 0, DIRECTION_RIGHT) {
    sprite = &ResrcManager::GetInstance().getTexture("REX_0");
    jumpCooldown = 0;
    hitCount = 0;
    originalSize = GetSize();
}

void Rex::UpdateStateAndPhysic() {
    const float deltaTime = GetFrameTime();
    if (hitCount >= 1) {
        SetVel(Vector2{0, 0});
        if (hitCount == 1) {
            sprite = &ResrcManager::GetInstance().getTexture("REX_1");
        } else if (hitCount >= 2) {
            sprite = &ResrcManager::GetInstance().getTexture("REX_2");
        }
        return;
    }
    if (GetState() == OBJECT_STATE_ON_GROUND) {
        float newVelX = GetVel().x;
        if (newVelX > 0 && newVelX > maxSpeedX) newVelX = maxSpeedX;
        if (newVelX < 0 && newVelX < -maxSpeedX) newVelX = -maxSpeedX;
        SetVel(Vector2{newVelX, GetVel().y});
        SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::getInstance().FIXED_TIME_STEP), GetPos().y});

        if (jumpCooldown <= 0 && rand() % 100 < 5) {
            SetVel(Vector2{GetVel().x, -200.0f});
            state = OBJECT_STATE_JUMPING;
            jumpCooldown = 100;
        }
    }

    if (GetVel().y > 0) {
        state = OBJECT_STATE_FALLING;
    }
    if (state == OBJECT_STATE_ON_GROUND) {
        SetVel(Vector2{GetVel().x, 0});
    }

    SetVel(Vector2{GetVel().x, GetVel().y + GameWorld::GetGravity() * deltaTime});

    if (jumpCooldown > 0) {
        jumpCooldown--;
    }

    static int updateCount = 0;
    const int updateThreshold = 50;

    if (fabs(GetVel().x) > 0.1f && hitCount == 0) {
        updateCount++;
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
    } else if (hitCount == 0) {
        if (GetDirection() == DIRECTION_RIGHT) {
            sprite = &ResrcManager::GetInstance().getTexture("REX_0_RIGHT");
        } else {
            sprite = &ResrcManager::GetInstance().getTexture("REX_0_LEFT");
        }
        textureIndex = 0;
        updateCount = 0;
    }
}

void Rex::OnHit() {
    hitCount++;
    if (hitCount == 1) {
        sprite = &ResrcManager::GetInstance().getTexture("REX_1");
        SetVel(Vector2{0, 0});
        float newHeight = originalSize.y * 0.66f;
        float heightReduction = originalSize.y - newHeight;
        SetSize(Vector2{originalSize.x, newHeight});
        SetPos(Vector2{GetPos().x, GetPos().y + heightReduction});
    } else if (hitCount == 2) {
        sprite = &ResrcManager::GetInstance().getTexture("REX_2");
        SetVel(Vector2{0, 0});
        float newHeight = originalSize.y * 0.33f;
        float heightReduction = (originalSize.y * 0.66f) - newHeight;
        SetSize(Vector2{originalSize.x, newHeight});
        SetPos(Vector2{GetPos().x, GetPos().y + heightReduction}); 
    }
}