#include "banzaibill.h"
#include "GameWorld.h"

BanzaiBill::BanzaiBill(Vector2 pos) 
    : Enemy(pos, Vector2{128, 128}, Vector2{1, 0}, GRAY, 0.2f, 0, DIRECTION_RIGHT) {
    sprite = &ResrcManager::GetInstance().getTexture("BANZAIBILL");
    maxSpeedX = 1.0f; // Tốc độ tối đa bằng vận tốc ban đầu
}

void BanzaiBill::UpdateStateAndPhysic() {
    const float deltaTime = GetFrameTime();
    // BanzaiBill bay thẳng, không chịu trọng lực
    SetPos(Vector2{(float)(GetPos().x + GetVel().x * GameClock::GetInstance().FIXED_TIME_STEP), GetPos().y});

    // Dùng texture cố định, không cần animation
    sprite = &ResrcManager::GetInstance().getTexture("BANZAIBILL");
    textureIndex = 0;
}

