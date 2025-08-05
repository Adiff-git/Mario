#include "../inc/Enemy/BuzzyBeetle.h"
#include "../inc/World/GameWorld.h"

BuzzyBeetle::BuzzyBeetle(Vector2 pos) 
    : Enemy(pos, Vector2{32, 32}, Vector2{20, 0}, BLUE, 0.2f, 0, DIRECTION_RIGHT) {
    sprite = &ResrcManager::GetInstance().getTexture("BuzzyBeetle_0_Right");
}

void BuzzyBeetle::UpdateStateAndPhysic() {
    UpdateDyingState();
    
    // Nếu đang DYING hoặc DEAD, không xử lý physics
    if (state == OBJECT_STATE_DYING || state == OBJECT_STATE_DEAD) {
        UpdateDeathEffect();
        return;
    }
    const float deltaTime = GetFrameTime();
    if (GetState() != OBJECT_STATE_ON_GROUND) {
        SetVel(Vector2{GetVel().x, GetVel().y + 9.81f * static_cast<float>(GameClock::GetInstance().FIXED_TIME_STEP)});
    }
    if (GetState() == OBJECT_STATE_ON_GROUND) {
        float newVelX = GetVel().x;
        if (newVelX > 0 && newVelX > maxSpeedX) newVelX = maxSpeedX;
        if (newVelX < 0 && newVelX < -maxSpeedX) newVelX = -maxSpeedX;
        SetVel(Vector2{newVelX, GetVel().y});
        SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::GetInstance().FIXED_TIME_STEP), GetPos().y});
    }
    if (vel.y > 0) {
        state = OBJECT_STATE_FALLING;
    }
    if (state == OBJECT_STATE_ON_GROUND) {
        vel.y = 0;
    }

    vel.y += GameWorld::GetGravity() * deltaTime;
    Object::UpdateStateAndPhysic();
    
    const int updateThreshold = 50;

    if (fabs(GetVel().x) > 0.1f) {
        updateCount++;
        if (updateCount >= updateThreshold) {
            if (GetDirection() == DIRECTION_RIGHT) {
                if (textureIndex == 0) {
                    sprite = &ResrcManager::GetInstance().getTexture("BuzzyBeetle_0_Right");
                    textureIndex = 1;
                } else {
                    sprite = &ResrcManager::GetInstance().getTexture("BuzzyBeetle_1_Right");
                    textureIndex = 0;
                }
            } else {
                if (textureIndex == 0) {
                    sprite = &ResrcManager::GetInstance().getTexture("BuzzyBeetle_0_Left");
                    textureIndex = 1;
                } else {
                    sprite = &ResrcManager::GetInstance().getTexture("BuzzyBeetle_1_Left");
                    textureIndex = 0;
                }
            }
            updateCount = 0;
        }
    } else {
        if (GetDirection() == DIRECTION_RIGHT) {
            sprite = &ResrcManager::GetInstance().getTexture("BuzzyBeetleTroopa_0");
        } else {
            sprite = &ResrcManager::GetInstance().getTexture("BuzzyBeetleTroopa_1");
        }
        textureIndex = 0;
        updateCount = 0;
    }
}