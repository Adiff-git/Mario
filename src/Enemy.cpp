#include "Enemy.h"
#include "ResrcManager.h"
#include "raylib.h"
#include "GameWorld.h"

Enemy::Enemy(Vector2 pos)
    : Object(pos, Vector2{32, 32}, Vector2{50, 0}, RED, 0.2f, 2, DIRECTION_RIGHT), maxSpeedX(20.0f), textureIndex(0) {
    sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_0");
}

void Enemy::Update() {
    UpdateStateAndPhysic();
}

void Enemy::UpdateStateAndPhysic() {
    const float deltaTime = GetFrameTime();
    if (GetState() != OBJECT_STATE_ON_GROUND) {
        SetVel(Vector2{GetVel().x, GetVel().y + 9.81f * (float)GameClock::getInstance().FIXED_TIME_STEP});
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
    UpdateCollisionProbes();
    
   static int updateCount = 0; 
    const int updateThreshold = 50; // Số lần gọi trước khi đổi texture (điều chỉnh tốc độ tại đây)

    if (fabs(GetVel().x) > 0.1f) { 
        updateCount++;
        if (updateCount >= updateThreshold) {
            if (textureIndex == 0) {
                sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_0");
                textureIndex = 1; // Chuyển sang GOOMBA_1
            } else if (textureIndex == 1) {
                sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_1");
                textureIndex = 0; // Quay lại GOOMBA_0
            }
            updateCount = 0;
        }
    } else {
        sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_0"); // Đứng yên giữ GOOMBA_0
        textureIndex = 0; 
        updateCount = 0; 
    }
}

void Enemy::draw() {
    if (sprite) {
        DrawTexture(*sprite, (int)GetPos().x, (int)GetPos().y, WHITE);
    } else {
        DrawRectangle((int)GetPos().x, (int)GetPos().y, (int)GetSize().x, (int)GetSize().y, GetColor());
    }
}

Goomba::Goomba(Vector2 pos) : Enemy(pos) {}