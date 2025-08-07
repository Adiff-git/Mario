#include "../inc/Enemy/RedKoopa.h"
#include "../inc/World/GameWorld.h"
#include <iostream>

RedKoopa::RedKoopa(Vector2 pos) 
    : Enemy(pos, Vector2{32, 48}, Vector2{-10, 0}, RED, 100.0f, 0, DIRECTION_LEFT) {
    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_0");
    jumpCooldown = 0;
    hitCount = 0;
    updateCount = 0;
    textureIndex = 0;
    originalSize = GetSize();
    isMoving = false;
}

void RedKoopa::EnterShell() {
    if (state != OBJECT_STATE_SHELL) {
        state = OBJECT_STATE_SHELL;
        SetVel(Vector2{0, GetVel().y});
        sprite = &ResrcManager::GetInstance().getTexture("SHELL_1");
        SetSize(Vector2{32, 32});
        textureIndex = 0;
        isMoving = false;
        std::cout << "[DEBUG] RedKoopa entered shell, state: " << state << ", vel.x: " << GetVel().x << ", hitCount: " << hitCount << ", isMoving: " << isMoving << std::endl;
    }
}

void RedKoopa::EnterShellWithVelocity(float velX) {
    if (state == OBJECT_STATE_SHELL && !isMoving) {
        SetVel(Vector2{velX, 0});
        sprite = &ResrcManager::GetInstance().getTexture("SHELL_1");
        SetSize(Vector2{32, 32});
        textureIndex = 0;
        isMoving = true;
        std::cout << "[DEBUG] RedKoopa shell moving, vel.x: " << GetVel().x << ", hitCount: " << hitCount << ", isMoving: " << isMoving << std::endl;
    }
}

void RedKoopa::OnHit(bool fromLeft) {
    hitCount++;
    if (hitCount == 1) {
        EnterShell();
        std::cout << "[DEBUG] Hit 1: Pos (" << GetPos().x << ", " << GetPos().y << "), Size (" << GetSize().x << ", " << GetSize().y << ")" << std::endl;
    } else if (hitCount == 2) {
        float shellSpeed = -150.0f;
        EnterShellWithVelocity(fromLeft ? -shellSpeed : shellSpeed);
        std::cout << "[DEBUG] Hit 2: Pos (" << GetPos().x << ", " << GetPos().y << "), Vel (" << GetVel().x << ", " << GetVel().y << ")" << std::endl;
    }
}

void RedKoopa::UpdateStateAndPhysic() {
    UpdateDyingState();

    if (state == OBJECT_STATE_DYING || state == OBJECT_STATE_DEAD) {
        UpdateDeathEffect();
        return;
    }

    const float deltaTime = GetFrameTime();
    if (state == OBJECT_STATE_SHELL) {
        // Thay đổi texture khi shell di chuyển
        if (isMoving) {
            updateCount++;
            const int updateThreshold = 10; // Điều chỉnh để thay đổi tốc độ animation
            if (updateCount >= updateThreshold) {
                textureIndex = (textureIndex + 1) % 4; // Chuyển từ 0 đến 3
                std::string textureName = "SHELL_" + std::to_string(textureIndex);
                sprite = &ResrcManager::GetInstance().getTexture(textureName);
                updateCount = 0;
                std::cout << "[DEBUG] Cập nhật texture shell thành: " << textureName << ", textureIndex: " << textureIndex << std::endl;
            }

            Vector2 newPos = Vector2{(double)(GetPos().x + GetVel().x * GameClock::GetInstance().FIXED_TIME_STEP), (double)(GetPos().y)};
            SetPos(newPos);
            std::cout << "[DEBUG] Shell di chuyển, pos.x: " << newPos.x << ", vel.x: " << GetVel().x << ", isMoving: " << isMoving << std::endl;
        } else {
            // Texture tĩnh khi shell không di chuyển
            sprite = &ResrcManager::GetInstance().getTexture("SHELL_0");
            SetVel(Vector2{0, GetVel().y});
        }

        // Áp dụng trọng lực nhưng giữ trạng thái OBJECT_STATE_SHELL
        if (GetState() != OBJECT_STATE_ON_GROUND) {
            SetVel(Vector2{GetVel().x, GetVel().y + GameWorld::GetGravity() * deltaTime});
        } else {
            SetVel(Vector2{GetVel().x, 0});
        }
        std::cout << "[DEBUG] Cập nhật: state=" << state << ", pos.x=" << GetPos().x << ", vel.x=" << GetVel().x << ", vel.y=" << GetVel().y << ", hitCount=" << hitCount << ", isMoving=" << isMoving << std::endl;
        return;
    }

    // Phần còn lại của phương thức giữ nguyên
    if (GetState() == OBJECT_STATE_ON_GROUND) {
        float newVelX = GetVel().x;
        if (newVelX > 0 && newVelX > maxSpeedX) newVelX = maxSpeedX;
        if (newVelX < 0 && newVelX < -maxSpeedX) newVelX = -maxSpeedX;
        SetVel(Vector2{newVelX, GetVel().y});
        SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::GetInstance().FIXED_TIME_STEP), GetPos().y});
    }
    if (vel.y == 0 && state != OBJECT_STATE_SHELL) {
        state = OBJECT_STATE_FALLING;
    }
    if (state == OBJECT_STATE_ON_GROUND) {
        SetVel(Vector2{GetVel().x, 0});
    }

    const int updateThreshold = 50;
    if (fabs(GetVel().x) > 0.1f && state != OBJECT_STATE_SHELL) {
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
    } else if (state != OBJECT_STATE_SHELL) {
        if (GetDirection() == DIRECTION_RIGHT) {
            sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_0_RIGHT");
        } else {
            sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_1_LEFT");
        }
        textureIndex = 0;
        updateCount = 0;
    }
}