#include "PiranhaPlant.h"
#include "ResrcManager.h"
#include "GameWorld.h"
#include "raylib.h"

PiranhaPlant::PiranhaPlant(Vector2 pos)
    : Enemy(pos, {32, 48}, {0, 0}, GREEN, 0.0f, 0, DIRECTION_UP),
      animationTimer(0.0f),
      animationInterval(0.2f),
      isMouthOpen(true)
{
    sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_MouthOpen");
}

void PiranhaPlant::UpdateStateAndPhysic() {
    const float deltaTime = GetFrameTime();
    animationTimer += deltaTime;

    static int updateCount = 0;
    const int updateThreshold = 17;  

    updateCount++;
    if (updateCount >= updateThreshold) {
        isMouthOpen = !isMouthOpen;
        sprite = &ResrcManager::GetInstance().getTexture(
            isMouthOpen ? "PiranhaPlant_MouthOpen" : "PiranhaPlant_MouthClosed"
        );
        updateCount = 0;
    }

    vel.y += GameWorld::GetGravity() * deltaTime;

    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;

    UpdateCollisionProbes();
}

