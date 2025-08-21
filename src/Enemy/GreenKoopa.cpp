#include "../inc/Enemy/GreenKoopa.h"
#include "../inc/World/GameWorld.h"
#include "../inc/World/GameClock.h"
#include <iostream>

GreenKoopa::GreenKoopa(Vector2 pos) 
    : Enemy(pos, Vector2{32, 48}, Vector2{-5, 0}, GREEN, 0.2f, 0, DIRECTION_LEFT) {
    sprite = &ResrcManager::GetInstance().getTexture("GreenKoopaTroopa_0_LEFT");
    jumpCooldown = 0;
    hitCount = 0;
    updateCount = 0;
    textureIndex = 0;
    originalSize = GetSize();
    isMoving = false;
}

void GreenKoopa::EnterShell() {
    if (AdditionalState != OBJECT_STATE_SHELL) {
        AdditionalState = OBJECT_STATE_SHELL;
        state = OBJECT_STATE_SHELL; // đồng bộ với MediatorCollision
        // Reset vận tốc để tránh “bắn” đi
        SetVel(Vector2{0, 0});

        sprite = &ResrcManager::GetInstance().getTexture("SHELL_0");

        // Thu nhỏ và dời Y theo chênh lệch chiều cao (pos là tâm)
        const float newH = 32.0f;
        const float oldH = GetSize().y; // 48 hiện tại
        SetSize(Vector2{32, 32});
        const float dy = (oldH - newH) * 0.5f; // bỏ +3 để tránh chui nền/đội nền
        SetPos(Vector2{GetPos().x, GetPos().y + dy});

        textureIndex = 0;
        isMoving = false;
        std::cout << "[DEBUG] GreenKoopa entered shell, state: " << state << ", vel.x: " << GetVel().x << ", pos.y: " << GetPos().y << ", hitCount: " << hitCount << ", isMoving: " << isMoving << std::endl;
    }
}

void GreenKoopa::EnterShellWithVelocity(float velX) {
    if (AdditionalState == OBJECT_STATE_SHELL && !isMoving) {
        // velX theo px/s
        SetVel(Vector2{velX, 0});
        sprite = &ResrcManager::GetInstance().getTexture("SHELL_0");
        textureIndex = 0;
        isMoving = true;
        std::cout << "[DEBUG] GreenKoopa shell moving, vel.x: " << GetVel().x << ", hitCount: " << hitCount << ", isMoving: " << isMoving << std::endl;
    }
}

void GreenKoopa::OnHit(bool fromLeft) {
    hitCount++;
    if (hitCount == 1) {
        EnterShell();
    } else if (hitCount == 2) {
        float shellSpeed = -150.0f;
        EnterShellWithVelocity(fromLeft ? -shellSpeed : shellSpeed);
    }
}

void GreenKoopa::UpdateStateAndPhysic() {
    UpdateDyingState();
    
    if (state == OBJECT_STATE_DYING || state == OBJECT_STATE_DEAD) {
        UpdateDeathEffect();
        return;
    }

    const float deltaTime = GetFrameTime();
    if (state == OBJECT_STATE_SHELL) {
        if (isMoving) {
            updateCount++;
            const int updateThreshold = 10;
            if (updateCount >= updateThreshold) {
                textureIndex = 0 + ((textureIndex + 1 - 0) % 4); // Chuyển đổi giữa 0, 1, 2, 3
                std::string textureName = "SHELL_" + std::to_string(textureIndex);
                sprite = &ResrcManager::GetInstance().getTexture(textureName);
                updateCount = 0;
                std::cout << "[DEBUG] Cập nhật texture shell thành: " << textureName << ", textureIndex: " << textureIndex << std::endl;
            }

            Vector2 newPos = Vector2{(double)(GetPos().x + GetVel().x * GameClock::GetInstance().FIXED_TIME_STEP), (double)(GetPos().y)};
            SetPos(newPos);
            std::cout << "[DEBUG] Shell di chuyển, pos.x: " << newPos.x << ", vel.x: " << GetVel().x << ", isMoving: " << isMoving << std::endl;
        } else {
            sprite = &ResrcManager::GetInstance().getTexture("SHELL_0");
            SetVel(Vector2{0, GetVel().y});
            textureIndex = 0;
        }

        if (GetState() != OBJECT_STATE_ON_GROUND) {
            SetVel(Vector2{GetVel().x, GetVel().y + GameWorld::GetGravity() * deltaTime});
        } else {
            SetVel(Vector2{GetVel().x, 0});
        }
        std::cout << "[DEBUG] Cập nhật: state=" << state << ", pos.x=" << GetPos().x << ", vel.x=" << GetVel().x << ", vel.y=" << GetVel().y << ", hitCount=" << hitCount << ", isMoving=" << isMoving << std::endl;
        return;
    }

    if (GetState() != OBJECT_STATE_ON_GROUND) {
        SetVel(Vector2{GetVel().x, GetVel().y + GameWorld::GetGravity() * static_cast<float>(GameClock::GetInstance().FIXED_TIME_STEP)});
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
    if (fabs(GetVel().x) > 0.1f && state != OBJECT_STATE_SHELL) {
        updateCount++;
        if (updateCount >= updateThreshold) {
            if (GetDirection() == DIRECTION_RIGHT) {
                if (textureIndex == 0) {
                    sprite = &ResrcManager::GetInstance().getTexture("GreenKoopaTroopa_0_RIGHT");
                    textureIndex = 1;
                } else {
                    sprite = &ResrcManager::GetInstance().getTexture("GreenKoopaTroopa_1_RIGHT");
                    textureIndex = 0;
                }
            } else {
                if (textureIndex == 0) {
                    sprite = &ResrcManager::GetInstance().getTexture("GreenKoopaTroopa_0_LEFT");
                    textureIndex = 1;
                } else {
                    sprite = &ResrcManager::GetInstance().getTexture("GreenKoopaTroopa_1_LEFT");
                    textureIndex = 0;
                }
            }
            updateCount = 0;
        }
    } else if (state != OBJECT_STATE_SHELL) {
        if (GetDirection() == DIRECTION_RIGHT) {
            sprite = &ResrcManager::GetInstance().getTexture("GreenKoopaTroopa_0_RIGHT");
        } else {
            sprite = &ResrcManager::GetInstance().getTexture("GreenKoopaTroopa_0_LEFT");
        }
        textureIndex = 0;
        updateCount = 0;
    }
}//aaahsk