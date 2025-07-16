#include "RedKoopa.h"
#include "GameWorld.h"
#include <iostream>

RedKoopa::RedKoopa(Vector2 pos) 
    : Enemy(pos, Vector2{32, 48}, Vector2{10, 0}, RED, 100.0f, 0, DIRECTION_RIGHT) {
    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaTroopa_0");
    jumpCooldown = 0;
    hitCount = 0;
    originalSize = GetSize();
    isMoving = false; // Ban đầu không di chuyển
}

void RedKoopa::EnterShell() {
    state = OBJECT_STATE_DYING;
    SetVel(Vector2{0, GetVel().y}); // Đứng im, giữ vận tốc y
    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaShell");
    textureIndex = 0;
    isMoving = false; // Khóa di chuyển sau lần đạp 1
    std::cout << "[DEBUG] RedKoopa entered shell, state: " << state << ", vel.x: " << GetVel().x << ", hitCount: " << hitCount << ", isMoving: " << isMoving << std::endl;
}

void RedKoopa::EnterShellWithVelocity(float velX) {
    state = OBJECT_STATE_DYING;
    SetVel(Vector2{velX, GetVel().y}); // Chạy với vận tốc mới
    sprite = &ResrcManager::GetInstance().getTexture("RedKoopaShell");
    textureIndex = 0;
    isMoving = true; // Cho phép di chuyển sau lần đạp 2
    std::cout << "[DEBUG] RedKoopa shell moving, vel.x: " << GetVel().x << ", hitCount: " << hitCount << ", isMoving: " << isMoving << std::endl;
}

void RedKoopa::OnHit(bool fromLeft) {
    hitCount++;
    if (hitCount == 1) {
        EnterShell(); // Lần 1: Chui vào mai, đứng im
        std::cout << "Hit 1: Pos (" << GetPos().x << ", " << GetPos().y << "), Size (" << GetSize().x << ", " << GetSize().y << ")" << std::endl;
    } else if (hitCount == 2) {
        float shellSpeed = 150.0f;
        if (fromLeft) {
            EnterShellWithVelocity(-shellSpeed); // Chạy trái nếu va chạm từ trái
        } else {
            EnterShellWithVelocity(shellSpeed); // Chạy phải nếu va chạm từ phải
        }
        std::cout << "Hit 2: Pos (" << GetPos().x << ", " << GetPos().y << "), Vel (" << GetVel().x << ", " << GetVel().y << ")" << std::endl;
    }
    // Không cần xóa trong OnHit, để MediatorCollision xử lý
}

void RedKoopa::UpdateStateAndPhysic() {
    const float deltaTime = GetFrameTime();
    if (state == OBJECT_STATE_DYING) {
        sprite = &ResrcManager::GetInstance().getTexture("RedKoopaShell");
        // Chỉ di chuyển nếu isMoving là true (sau hit 2)
        if (isMoving) {
            SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::getInstance().FIXED_TIME_STEP), GetPos().y});
        } else {
            SetVel(Vector2{0, GetVel().y}); // Khóa vel.x = 0 sau hit 1
        }
        if (GetState() != OBJECT_STATE_ON_GROUND) {
            SetVel(Vector2{GetVel().x, GetVel().y + 9.81f * static_cast<float>(GameClock::getInstance().FIXED_TIME_STEP)});
        }
        if (GetState() == OBJECT_STATE_ON_GROUND) {
            SetVel(Vector2{GetVel().x, 0});
        }
        SetVel(Vector2{GetVel().x, GetVel().y + GameWorld::GetGravity() * deltaTime});
        std::cout << "[DEBUG] Update: state=" << state << ", vel.x=" << GetVel().x << ", hitCount=" << hitCount << ", isMoving=" << isMoving << std::endl;
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
    if (GetVel().y > 0) {
        state = OBJECT_STATE_FALLING;
    }
    if (state == OBJECT_STATE_ON_GROUND) {
        SetVel(Vector2{GetVel().x, 0});
    }
    vel.y += GameWorld::GetGravity() * deltaTime;
    Object::UpdateStateAndPhysic();
    
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