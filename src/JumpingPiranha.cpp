#include "JumpingPiranha.h"
#include "ResrcManager.h"
#include "GameWorld.h"
#include "GameClock.h"

JumpingPiranhaPlant::JumpingPiranhaPlant(Vector2 pos)
    : Enemy(pos, {32, 48}, {0, 0}, GREEN, 0.0f, 0, DIRECTION_UP),
      animationTimer(0.0f),
      animationInterval(0.2f),
      textureIndex(0),
      directionAnim(1)
{
    sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_0");
}

void JumpingPiranhaPlant::UpdateStateAndPhysic() {
    float deltaTime = GetFrameTime();

    if (GetState() != OBJECT_STATE_ON_GROUND) {
        vel.y += 9.81f * static_cast<float>(GameClock::getInstance().FIXED_TIME_STEP);
    }
    if (GetState() == OBJECT_STATE_ON_GROUND) {
        if (vel.x > maxSpeedX) vel.x = maxSpeedX;
        if (vel.x < -maxSpeedX) vel.x = -maxSpeedX;
        pos.x += vel.x * static_cast<float>(GameClock::getInstance().FIXED_TIME_STEP);
    }

    if (vel.y > 0) state = OBJECT_STATE_FALLING;
    if (state == OBJECT_STATE_ON_GROUND) vel.y = 0;

    vel.y += GameWorld::GetGravity() * deltaTime;

    animationTimer += deltaTime;
    if (animationTimer >= animationInterval) {
        textureIndex += directionAnim;

        if (textureIndex >= 3) {
            textureIndex = 3;
            directionAnim = -1;
        } else if (textureIndex <= 0) {
            textureIndex = 0;
            directionAnim = 1;
        }

        sprite = &ResrcManager::GetInstance().getTexture("PiranhaPlant_" + std::to_string(textureIndex));
        animationTimer = 0.0f;
    }

    pos.x += vel.x * deltaTime;
    pos.y += vel.y * deltaTime;

    UpdateCollisionProbes();
}
