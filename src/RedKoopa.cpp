#include "RedKoopa.h"
#include "GameWorld.h"
#include <iostream>

RedKoopa::RedKoopa(Vector2 pos) 
    : Enemy(pos, Vector2{32, 48}, Vector2{10, 0}, RED, 100.0f, 0, DIRECTION_RIGHT) {
    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_0");
}

void RedKoopa::EnterShell() {
    state = OBJECT_STATE_DYING;
    vel.x = 0;
    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaShell");
    textureIndex = 0;
    std::cout << "[DEBUG] RedKoopa entered shell, state: " << state << ", vel.x: " << vel.x << ", frame: " << std::endl;
}

void RedKoopa::EnterShellWithVelocity(float velX) {
    state = OBJECT_STATE_DYING;
    vel.x = velX;
    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaShell");
    textureIndex = 0;
    std::cout << "[DEBUG] RedKoopa shell moving, vel.x: " << vel.x << ", frame: " << std::endl;
}

void RedKoopa::HandleCollisionWithMario(float marioX) {
    static float accumulatedTime = 0.0f; 
    static float lastCollisionTime = 0.0f; 
    float deltaTime = GetFrameTime();
    accumulatedTime += deltaTime; 

    if (accumulatedTime - lastCollisionTime > 1.0f) {
        if (state != OBJECT_STATE_DYING) {
    
            EnterShell();
          
        } else {

            float koopaX = GetPos().x;
            float shellSpeed = 150.0f; 
            if (marioX < koopaX) {

                EnterShellWithVelocity(shellSpeed);

            } else {

                EnterShellWithVelocity(-shellSpeed);
                std::cout << "[DEBUG] Second hit: Shell moves left" << std::endl;
            }
        }
        lastCollisionTime = accumulatedTime;
    }
}

void RedKoopa::UpdateStateAndPhysic() {
    const float deltaTime = GetFrameTime();
    if (state == OBJECT_STATE_DYING) {

        sprite = &ResrcManager::GetInstance().getTexture("RedKoopaShell");
        SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::getInstance().FIXED_TIME_STEP), GetPos().y});
        if (GetState() != OBJECT_STATE_ON_GROUND) {
            SetVel(Vector2{GetVel().x, GetVel().y + 9.81f * static_cast<float>(GameClock::getInstance().FIXED_TIME_STEP)});
        }
        if (GetState() == OBJECT_STATE_ON_GROUND) {
            vel.y = 0;
        }
        vel.y += GameWorld::GetGravity() * deltaTime;
        return;
    }

    if (GetState() != OBJECT_STATE_ON_GROUND) {
        SetVel(Vector2{GetVel().x, GetVel().y + 9.81f * static_cast<float>(GameClock::getInstance().FIXED_TIME_STEP)});
    }
    if (GetState() == OBJECT_STATE_ON_GROUND) {
        float newVelX = GetVel().x;
        if (newVelX > 0 && newVelX > maxSpeedX) newVelX = maxSpeedX;
        if (newVelX < 0 && newVelX < -maxSpeedX) newVelX = -maxSpeedX;
        SetVel(Vector2{newVelX, GetVel().y});
        SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::getInstance().FIXED_TIME_STEP), GetPos().y});
    }
    if (vel.y > 0) {
        state = OBJECT_STATE_FALLING;
    }
    if (state == OBJECT_STATE_ON_GROUND) {
        vel.y = 0;
    }

    vel.y += GameWorld::GetGravity() * deltaTime;

    static int updateCount = 0;
    const int updateThreshold = 50;

    if (fabs(GetVel().x) > 0.1f && state != OBJECT_STATE_DYING) {
        updateCount++;
        if (updateCount >= updateThreshold) {
            if (GetDirection() == DIRECTION_RIGHT) {
                if (textureIndex == 0) {
                    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_0_RIGHT");
                    textureIndex = 1;
                } else {
                    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_1_RIGHT");
                    textureIndex = 0;
                }
            } else {
                if (textureIndex == 0) {
                    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_0_LEFT");
                    textureIndex = 1;
                } else {
                    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_1_LEFT");
                    textureIndex = 0;
                }
            }
            updateCount = 0;
        }
    } else if (state != OBJECT_STATE_DYING) {
        if (GetDirection() == DIRECTION_RIGHT) {
            sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_0");
        } else {
            sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_1");
        }
        textureIndex = 0;
        updateCount = 0;
    }
}