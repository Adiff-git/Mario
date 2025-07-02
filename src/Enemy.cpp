#include "Enemy.h"
#include "ResrcManager.h"
#include "raylib.h"

Enemy::Enemy(Vector2 pos)
    : Object(pos, Vector2{32, 32}, Vector2{50, 0}, RED, 0.2f, 2, DIRECTION_RIGHT), maxSpeedX(1.0f) {
    sprite = &ResrcManager::GetInstance().getTexture("GOOMBA_0"); // Sử dụng texture tạm
}

void Enemy::Update() {
    UpdateStateAndPhysic();
}

void Enemy::UpdateStateAndPhysic() {
    if (GetState() != OBJECT_STATE_ON_GROUND) {
        SetVel(Vector2{GetVel().x, GetVel().y + 9.81f * (float)GameClock::getInstance().FIXED_TIME_STEP}); // Áp dụng trọng lực
    }
    if (GetState() == OBJECT_STATE_ON_GROUND) {
        float newVelX = GetVel().x;
        if (newVelX > 0 && newVelX > maxSpeedX) newVelX = maxSpeedX;
        if (newVelX < 0 && newVelX < -maxSpeedX) newVelX = -maxSpeedX;
        SetVel(Vector2{newVelX, GetVel().y});
        SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::getInstance().FIXED_TIME_STEP), GetPos().y});
    }
    UpdateCollisionProbes(); // Cập nhật probe va chạm
}

void Enemy::draw() {
    if (sprite) {
        DrawTexture(*sprite, (int)GetPos().x, (int)GetPos().y, WHITE);
    } else {
        DrawRectangle((int)GetPos().x, (int)GetPos().y, (int)GetSize().x, (int)GetSize().y, GetColor());
    }
}

Goomba::Goomba(Vector2 pos) : Enemy(pos) {}