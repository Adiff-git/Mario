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
      originalPos(pos),
      moveTimer(0.0f),
      moveSpeed(50.0f),      
      maxHeight(64.0f),      
      isMovingUp(false),     
      isVisible(true)
{
    sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_MouthOpen");
    this->pos = pos;
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

    const int updateThreshold = 17;  
    updateCount++;
    if (updateCount >= updateThreshold) {
        isMouthOpen = !isMouthOpen;
        sprite = &ResrcManager::GetInstance().getTexture(
            isMouthOpen ? "PiranhaPlant_MouthOpen" : "PiranhaPlant_MouthClosed"
        );
        updateCount = 0;
    }

    const float cycleDuration = 4.0f;
    const float moveUpDuration = 1.5f;
    const float pauseDuration = 1.0f;
    const float moveDownDuration = 1.5f;
    
    if (moveTimer >= cycleDuration) {
        moveTimer = 0.0f;
    }
    
    if (moveTimer < moveUpDuration) {
        isMovingUp = true;
        vel.y = -moveSpeed;
    } else if (moveTimer < moveUpDuration + pauseDuration) {
        vel.y = 0;
    } else {
        isMovingUp = false;
        vel.y = moveSpeed;
    }
    
    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;
    
    static int debugCounter = 0;
    debugCounter++;
    
    float minY = originalPos.y - maxHeight;
    float maxY = originalPos.y + maxHeight;
    
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
//zjfgjsgfksfd
//jhasjgfdskjgfdfjgh
///àgjajafuitauạgfjafjfjjggmn
//jagfjkgafakfakhfaghfajajksfjhsdff