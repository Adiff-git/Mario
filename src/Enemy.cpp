
#include "Enemy.h"
#include "ResrcManager.h"
#include "raylib.h"
#include "GameWorld.h"

Enemy::Enemy(Vector2 pos)
    : Object(pos, Vector2{32, 32}, Vector2{50, 0}, RED, 0.2f, 2, DIRECTION_RIGHT), maxSpeedX(20.0f), textureIndex(0) {
    sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_0");
    // Tùy chỉnh kích thước probe
    cpN.setSize(Vector2{size.x/2, 1}); 
    cpS.setSize(Vector2{size.x/2, 1}); 
    cpE.setSize(Vector2{5, size.y - 5});
    cpW.setSize(Vector2{5, size.y - 5});
    cpN.setColor(RED);
    cpS.setColor(RED);
    cpE.setColor(RED);
    cpW.setColor(RED);
    UpdateCollisionProbes();
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
    const int updateThreshold = 50; // Số lần gọi trước khi đổi texture

    if (fabs(GetVel().x) > 0.1f) { 
        updateCount++;
        if (updateCount >= updateThreshold) {
            if (direction == DIRECTION_RIGHT) {
                if (textureIndex == 0) {
                    sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_0_RIGHT");
                    textureIndex = 1;
                } else if (textureIndex == 1) {
                    sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_1_RIGHT");
                    textureIndex = 0;
                }
            } else if (direction == DIRECTION_LEFT) {
                if (textureIndex == 0) {
                    sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_0_LEFT");
                    textureIndex = 1;
                } else if (textureIndex == 1) {
                    sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_1_LEFT");
                    textureIndex = 0;
                }
            }
            updateCount = 0;
        }
    } else {
        if (direction == DIRECTION_RIGHT) {
            sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_0");
        } else if (direction == DIRECTION_LEFT) {
            sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_1");
        }
        textureIndex = 0;
        updateCount = 0;
    }
}

void Enemy::UpdateCollisionProbes() {
    cpN.setPos(Vector2{ pos.x + size.x / 2 - cpN.getSize().x/2, pos.y });
    cpS.setPos(Vector2{ pos.x + size.x / 2 - cpS.getSize().x/2, pos.y + size.y - cpS.getSize().y + 1 });
    cpE.setPos(Vector2{ pos.x + size.x - cpE.getSize().x, pos.y + size.y / 2 - cpE.getSize().y / 2 }); // Sát mép phải
    cpW.setPos(Vector2{ pos.x, pos.y + size.y / 2 - cpW.getSize().y / 2 }); // Sát mép trái
}
void Enemy::drawCollisionProbes() {
    cpN.draw();
    cpS.draw();
    cpE.draw();
    cpW.draw();
}

void Enemy::draw() {
    if (sprite) {
        DrawTexture(*sprite, (int)GetPos().x, (int)GetPos().y, WHITE);
    } else {
        DrawRectangle((int)GetPos().x, (int)GetPos().y, (int)GetSize().x, (int)GetSize().y, GetColor());
    }
    drawCollisionProbes();
}

Goomba::Goomba(Vector2 pos) : Enemy(pos) {}
