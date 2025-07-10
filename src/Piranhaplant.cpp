#include "PiranhaPlant.h"
#include "ResrcManager.h"
#include "GameWorld.h"
#include "raylib.h"

PiranhaPlant::PiranhaPlant(Vector2 pos)
    : Enemy(pos, Vector2{32, 48}, Vector2{0, 0}, GREEN, 0.0f, 0, DIRECTION_UP)
{
    sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_MouthOpen");
}

void PiranhaPlant::UpdateStateAndPhysic() {
    float deltaTime = GetFrameTime();
    animationTimer += deltaTime;

    if (animationTimer >= animationInterval) {
        isMouthOpen = !isMouthOpen;
        sprite = &ResrcManager::GetInstance().getTexture(
            isMouthOpen ? "PiranhaPlant_MouthOpen" : "PiranhaPlant_MouthClosed"
        );
        animationTimer = 0.0f;
    }

    // Không di chuyển, nhưng vẫn cập nhật vị trí nếu có hiệu ứng game
    pos.y += vel.y * deltaTime;
    pos.x += vel.x * deltaTime;

    // Update các vùng va chạm
    UpdateCollisionProbes();
}
