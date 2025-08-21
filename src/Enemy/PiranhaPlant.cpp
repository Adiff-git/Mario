#include "../inc/Enemy/PiranhaPlant.h"
#include "ResrcManager.h"
#include "../inc/World/GameWorld.h"
#include "raylib.h"
#include <cmath>
#include <cstdio>

PiranhaPlant::PiranhaPlant(Vector2 pos)
    : Enemy(pos, {32, 48}, {0, 0}, GREEN, 0.0f, 0, DIRECTION_UP),
      animationTimer(0.0f),
      animationInterval(0.2f),
      isMouthOpen(true),
      originalPos({pos.x, pos.y + 32}),   // dịch xuống 32px
      moveTimer(0.0f),
      moveSpeed(50.0f),
      maxHeight(64.0f),
      isMovingUp(false),
      isVisible(true)
{
    // Spawn ở vị trí cao nhất (cũng đã dịch xuống 32px)
    this->pos = { pos.x, originalPos.y - maxHeight };

    sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_MouthOpen");
}

void PiranhaPlant::UpdateStateAndPhysic() {
    UpdateDyingState();

    if (state == OBJECT_STATE_DYING || state == OBJECT_STATE_DEAD) {
        UpdateDeathEffect();
        return;
    }

    const float deltaTime = 0.016f;
    animationTimer += deltaTime;
    moveTimer += deltaTime;

    // --- Animation miệng há / đóng ---
    const int updateThreshold = 17;
    updateCount++;
    if (updateCount >= updateThreshold) {
        isMouthOpen = !isMouthOpen;
        sprite = &ResrcManager::GetInstance().getTexture(
            isMouthOpen ? "PiranhaPlant_MouthOpen" : "PiranhaPlant_MouthClosed"
        );
        updateCount = 0;
    }

    // --- Chu kỳ chuyển động ---
    const float moveDownDuration = 1.5f;
    const float pauseDuration    = 1.0f;
    const float moveUpDuration   = 1.5f;
    const float cycleDuration    = moveDownDuration + pauseDuration + moveUpDuration;

    if (moveTimer >= cycleDuration) {
        moveTimer = 0.0f;
    }

    // Xuống -> dừng -> lên
    if (moveTimer < moveDownDuration) {
        isMovingUp = false;
        vel.y = moveSpeed; // đi xuống
    } else if (moveTimer < moveDownDuration + pauseDuration) {
        vel.y = 0;         // dừng lại
    } else {
        isMovingUp = true;
        vel.y = -moveSpeed; // đi lên
    }

    // --- Cập nhật vị trí ---
    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;

    // --- Giới hạn trên/dưới (đã dịch xuống 32px nhờ originalPos) ---
    float minY = originalPos.y - maxHeight; // cao nhất
    float maxY = originalPos.y;             // thấp nhất (gốc pipe + 32)

    if (pos.y <= minY) {
        pos.y = minY;
        vel.y = 0;
    }
    if (pos.y >= maxY) {
        pos.y = maxY;
        vel.y = 0;
    }

    UpdateCollisionProbes();
}
