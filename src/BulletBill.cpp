#include "BulletBill.h"
#include "GameWorld.h"

BulletBill::BulletBill(Vector2 pos) 
    : Enemy(pos, Vector2{32, 16}, Vector2{1, 0}, GRAY, 0.2f, 0, DIRECTION_RIGHT) {
    sprite = &ResrcManager::GetInstance().getTexture("bulletbill");
    maxSpeedX = 1.0f; // Tốc độ tối đa bằng vận tốc ban đầu
}

void BulletBill::UpdateStateAndPhysic() {
    const float deltaTime = GetFrameTime();
    // BulletBill bay thẳng, không chịu trọng lực
    SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::getInstance().FIXED_TIME_STEP), GetPos().y});

    // Dùng texture cố định, không cần animation
    sprite = &ResrcManager::GetInstance().getTexture("BulletBill");
    textureIndex = 0;
}