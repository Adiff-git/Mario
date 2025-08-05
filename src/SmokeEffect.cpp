#include "../inc/SmokeEffect.h"

SmokeEffect::SmokeEffect(Vector2 pos) 
    : Object(pos, Vector2{32, 32}, WHITE),
      currentFrame(0),
      maxFrames(4),
      frameTime(0.1f),
      frameAcumulator(0.0f),
      lifeTime(0.8f),
      lifeTimeAcum(0.0f),
      alpha(1.0f),
      isActive(true)
{
    // Load smoke textures
    smokeFrames.push_back(&ResrcManager::GetInstance().getTexture("SMOKE_0"));
    smokeFrames.push_back(&ResrcManager::GetInstance().getTexture("SMOKE_1"));
    smokeFrames.push_back(&ResrcManager::GetInstance().getTexture("SMOKE_2"));
    smokeFrames.push_back(&ResrcManager::GetInstance().getTexture("SMOKE_3"));

    // smokeFrames.push_back(&ResrcManager::GetInstance().getTexture("COIN_0"));
    // smokeFrames.push_back(&ResrcManager::GetInstance().getTexture("COIN_1"));
    // smokeFrames.push_back(&ResrcManager::GetInstance().getTexture("COIN_2"));
    // smokeFrames.push_back(&ResrcManager::GetInstance().getTexture("COIN_3"));
    
    state = OBJECT_STATE_ACTIVE;
}

void SmokeEffect::Update() {
    if (!isActive) return;
    
    float dt = GameClock::GetInstance().FIXED_TIME_STEP;
    
    // Update frame animation
    frameAcumulator += dt;
    if (frameAcumulator >= frameTime) {
        frameAcumulator = 0.0f;
        if (currentFrame < maxFrames - 1) { // ✅ Chỉ tăng khi chưa đến frame cuối
            currentFrame++;
        }
        // Nếu đã ở frame cuối thì giữ nguyên
    }
    
    // Update lifetime and alpha
    lifeTimeAcum += dt;
    float progress = lifeTimeAcum / lifeTime;
    alpha = 1.0f - progress;
    
    if (lifeTimeAcum >= lifeTime) {
        isActive = false;
        state = OBJECT_STATE_TO_BE_REMOVED;
    }
}

void SmokeEffect::Draw() {
    if (!isActive) return;
    
    Update(); // Thêm update để đảm bảo animation chạy
    
    bool textureDrawn = false;
    
    // Thử vẽ texture nếu có
    if (!smokeFrames.empty() && currentFrame < smokeFrames.size()) {
        Texture2D* currentTexture = smokeFrames[currentFrame];
        if (currentTexture && currentTexture->id != 0) {  // ✅ Kiểm tra cả pointer và ID
            Color drawColor = WHITE;  // Đổi thành WHITE thay vì YELLOW
            drawColor.a = (unsigned char)(255 * alpha);
            DrawTextureEx(*currentTexture, pos, 0.0f, 1.0f, drawColor);
            textureDrawn = true;
        }
    }
    // Fallback: Vẽ hình chữ nhật nếu texture không vẽ được
    if (!textureDrawn) {
        Color rectColor = YELLOW;  // Màu vàng để dễ phân biệt
        rectColor.a = (unsigned char)(255 * alpha);
        DrawRectangle((int)pos.x, (int)pos.y, 32, 32, rectColor);
    }
}

void SmokeEffect::UpdateStateAndPhysic() {
    Update();
}