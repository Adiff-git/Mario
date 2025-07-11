#include "GameWorld.h"
#include "Mario.h"
#include "raylib.h"
#include "ResrcManager.h"
#include "Start.h"
#include "GameClock.h"

Start::Start(Vector2 pos)
    : Item(pos, {32, 32}, {100, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      applyGravity(true), isMoving(true)
{
    state = OBJECT_STATE_ACTIVE;
    sprite = &ResrcManager::GetInstance().getTexture("star");
}

void Start::updateMario(Mario& mario)
{
    mario.setInvincible(true);
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void Start::playCollisionSound()
{
    // Optional
}

void Start::Update()
{
    float dt = GetFrameTime();
    float fixedDt = GameClock::getInstance().FIXED_TIME_STEP;

    if (state != OBJECT_STATE_ACTIVE)
        return;

    // Áp dụng trọng lực
    if (applyGravity)
        vel.y += GameWorld::GetGravity() * dt;

    // Nếu chạm đất thì bật ngược lại (nhảy)
    if (state == OBJECT_STATE_ON_GROUND) {
        vel.y = -250.0f;  // độ cao bật lên (tùy chỉnh nếu cần)
    }

    // Cập nhật trạng thái rơi
    if (vel.y > 0)
        state = OBJECT_STATE_FALLING;

    // Di chuyển
    pos.x += vel.x * fixedDt;
    pos.y += vel.y * dt;

    UpdateCollisionProbes();
}

void Start::draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (sprite)
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
}
