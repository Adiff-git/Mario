#include "../inc/Enemy/BulletBill.h"
#include "../inc/World/GameWorld.h"

BulletBill::BulletBill(Vector2 pos) 
    : Enemy(pos, Vector2{32, 16}, Vector2{-100, 0}, GRAY, 0.2f, 0, DIRECTION_LEFT) {
    sprite = &ResrcManager::GetInstance().getTexture("BULLETBILL_LEFT");
    maxSpeedX = 10.0f;
}

void BulletBill::UpdateStateAndPhysic()  {
    UpdateDyingState();
    
    if (state == OBJECT_STATE_DYING || state == OBJECT_STATE_DEAD) {
        UpdateDeathEffect();
        return;
    }
    const float deltaTime = GetFrameTime();
    
    // Apply speed multiplier to velocity
    Vector2 adjustedVel = Vector2{vel.x * speedMultiplier, vel.y};
    SetPos(Vector2{(float)(GetPos().x + adjustedVel.x * GameClock::GetInstance().FIXED_TIME_STEP), GetPos().y});

    sprite = &ResrcManager::GetInstance().getTexture("BULLETBILL_LEFT");
    textureIndex = 0;
}