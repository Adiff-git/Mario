#include "FlyingGoomba.h"
#include "GameClock.h"
#include "GameWorld.h"
#include "ResrcManager.h"

FlyingGoomba::FlyingGoomba(Vector2 pos)
    : Enemy(pos, Vector2{48, 32}, Vector2{0, 0}, BROWN, 0.2f, 0, DIRECTION_RIGHT)
{
    sprite = &ResrcManager::GetInstance().getTexture("FlyingGoomba_Flap_Right");

    vel.x = 2.7f;           
    maxSpeedX = 3.0f;        
    textureIndex = 0;
}

void FlyingGoomba::UpdateStateAndPhysic() {
    float deltaTime = GetFrameTime();

    vel.y += GameWorld::GetGravity() * deltaTime;

    // Cập nhật hướng bay
    if (vel.x > 0) direction = DIRECTION_RIGHT;
    else if (vel.x < 0) direction = DIRECTION_LEFT;

    // Giới hạn vận tốc
    if (vel.x > maxSpeedX) vel.x = maxSpeedX;
    if (vel.x < -maxSpeedX) vel.x = -maxSpeedX;

    // Cập nhật vị trí
    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;

    // Animation đơn giản
    static int updateCount = 0;
    const int updateThreshold = 30;
    updateCount++;

    if (updateCount >= updateThreshold) {
        if (direction == DIRECTION_RIGHT) {
            sprite = &ResrcManager::GetInstance().getTexture(
                textureIndex == 0 ? "FlyingGoomba_Flap_Right" : "FlyingGoomba_WingDown_Right");
        } else {
            sprite = &ResrcManager::GetInstance().getTexture(
                textureIndex == 0 ? "FlyingGoomba_Flap_Center" : "FlyingGoomba_WingDown_Center");
        }

        textureIndex = 1 - textureIndex;
        updateCount = 0;
    }
}

