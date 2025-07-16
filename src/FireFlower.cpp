#include "FireFlower.h"
#include "raylib.h"
#include "ResrcManager.h"

FireFlower::FireFlower(Vector2 pos)
    : Item(pos, {32, 32}, {0, 0}, WHITE, 0.8f, 2, DIRECTION_RIGHT, 1, 0),
      blinking(false), blinkingAcum(0), blinkingTime(0.1f), doBlink(false)
{
    state = OBJECT_STATE_ACTIVE;
}

void FireFlower::updateMario(Mario& mario)
{
    if (state == OBJECT_STATE_TO_BE_REMOVED) return;

    mario.changetoFire();                      // Mario chuyển sang trạng thái bắn lửa
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}


void FireFlower::playCollisionSound()
{
    // TODO: play sound
}

void FireFlower::Update()
{
    const float animSpeed = 0.6f; // mỗi frame 0.6s
    static float animTime = 0.0f;
    animTime += GameClock::GetInstance().FIXED_TIME_STEP;

    if (animTime >= animSpeed) {
        currentFrame = (currentFrame + 1) % maxFrames;
        animTime = 0.0f;
    }

    UpdateCollisionProbes();
}

void FireFlower::Draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (blinking && doBlink)
        return;

    // Dùng currentFrame để lấy tên ảnh tương ứng
    std::string texKey = "FIRE FLOWER_" + std::to_string(currentFrame);
    Texture2D& tex = ResrcManager::GetInstance().getTexture(texKey);

    if (tex.id == 0) {
        TraceLog(LOG_ERROR, "Texture %s not loaded!", texKey.c_str());
        return;
    }

    DrawTextureEx(tex, pos, 0.0f, 1.0f, color);
}