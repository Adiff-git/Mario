#include "../inc/Item/CourseClearToken.h"
#include "ResrcManager.h"
#include "../inc/Character/Character.h"
#include "raylib.h"

CourseClearToken::CourseClearToken(Vector2 pos)
    : Item(
        pos,
        {32, 32},           // Kích thước
        {0, 0},             // Không di chuyển
        WHITE,              // Màu vẽ
        0.0f,               // Không dùng frame animation
        1,                  // 1 frame
        DIRECTION_RIGHT,    // Không quan trọng
        1,                  // 1 lần va chạm là biến mất
        0                   // Không cộng điểm
    )
{
    // Lấy texture từ ResrcManager
    sprite = &ResrcManager::GetInstance().getTexture("COURSE CLEAR TOKEN");

    if (!sprite || sprite->id == 0 || sprite->width == 0 || sprite->height == 0) {
        TraceLog(LOG_ERROR, "[CourseClearToken] Texture 'COURSE CLEAR TOKEN' failed to load or not initialized!");
    } else {
        TraceLog(LOG_INFO, "[CourseClearToken] Texture loaded: %dx%d",
                 sprite->width, sprite->height);
    }

    state = OBJECT_STATE_ACTIVE;
}

void CourseClearToken::Update() {
    UpdateCollisionProbes();
    // Không còn hiệu ứng xoay nên không cần tính gì thêm
}

void CourseClearToken::Draw() {
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    // Nếu texture lỗi → fallback
    if (!sprite || sprite->id == 0 || sprite->width == 0) {
        DrawRectangleV(pos, size, RED);
        return;
    }

    Rectangle source = {
        0, 0,
        (float)sprite->width,
        (float)sprite->height
    };

    Rectangle dest = {
        pos.x,
        pos.y,
        size.x,
        size.y
    };

    DrawTexturePro(
        *sprite,
        source,
        dest,
        Vector2{0, 0},   // vẽ từ góc trái trên
        0.0f,
        color
    );
}

void CourseClearToken::updateMario(Character& mario) {
    mario.Victory();
    mario.SetState(OBJECT_STATE_VICTORY);
}

void CourseClearToken::playCollisionSound() {
    // TODO: Thêm âm thanh nếu cần
}
