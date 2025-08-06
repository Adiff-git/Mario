#include "../inc/ScoreEffect.h"
#include "../inc/World/GameClock.h"
#include "../inc/Character/Character.h" // Include để sử dụng Character

// Static variable definition
Character* ScoreEffect::currentPlayer = nullptr;

ScoreEffect::ScoreEffect(Vector2 pos, int score) 
    : SmokeEffect(pos), scoreValue(score), velocity{0, -50.0f}
{
    // Thiết lập điểm số dựa trên giá trị và cộng điểm cho player
    switch(score) {
        case 100:
            scoreText = "100";
            if (currentPlayer) currentPlayer->AddScore(100);
            break;
        case 200:
            scoreText = "200";
            if (currentPlayer) currentPlayer->AddScore(200);
            break;
        case 400:
            scoreText = "400";
            if (currentPlayer) currentPlayer->AddScore(400);
            break;
        case 800:
            scoreText = "800";
            if (currentPlayer) currentPlayer->AddScore(800);
            break;
        case 1000:
            scoreText = "1000";
            if (currentPlayer) currentPlayer->AddScore(1000);
            break;
        case 2000:
            scoreText = "2000";
            if (currentPlayer) currentPlayer->AddScore(2000);
            break;
        case 4000:
            scoreText = "4000";
            if (currentPlayer) currentPlayer->AddScore(4000);
            break;
        case 8000:
            scoreText = "8000";
            if (currentPlayer) currentPlayer->AddScore(8000);
            break;
        default:
            scoreText = std::to_string(score);
            if (currentPlayer) currentPlayer->AddScore(score);
            break;
    }
    
    lifeTime = 1.5f;
    
    state = OBJECT_STATE_ACTIVE;
}

void ScoreEffect::Update() {
    if (!isActive) return;
    
    float dt = GameClock::GetInstance().FIXED_TIME_STEP;
    
    pos.y += velocity.y * dt;
    
    velocity.y *= 0.98f;
    
    // Update lifetime và alpha
    lifeTimeAcum += dt;
    float progress = lifeTimeAcum / lifeTime;
    alpha = 1.0f - progress;
    
    if (lifeTimeAcum >= lifeTime) {
        isActive = false;
        state = OBJECT_STATE_TO_BE_REMOVED;
    }
}

void ScoreEffect::Draw() {
    if (!isActive) return;
    
    Update();
    
    Color textColor = WHITE;  // Đổi từ YELLOW sang WHITE
    textColor.a = (unsigned char)(255 * alpha);
    
    int fontSize = 24;  // Tăng size cơ bản
    if (scoreValue >= 1000) fontSize = 28;
    if (scoreValue >= 4000) fontSize = 32;
    
    Color outlineColor = BLACK;
    outlineColor.a = (unsigned char)(255 * alpha);
    
    // Vẽ outline với nhiều layer để dày hơn
    for (int x = -2; x <= 2; x++) {
        for (int y = -2; y <= 2; y++) {
            if (x != 0 || y != 0) {  // Không vẽ ở vị trí chính giữa
                DrawText(scoreText.c_str(), (int)pos.x + x, (int)pos.y + y, fontSize, outlineColor);
            }
        }
    }
    
    Color shadowColor = GRAY;
    shadowColor.a = (unsigned char)(128 * alpha);  // Trong suốt hơn
    DrawText(scoreText.c_str(), (int)pos.x + 1, (int)pos.y + 2, fontSize, shadowColor);
    
    // Vẽ text chính màu trắng
    DrawText(scoreText.c_str(), (int)pos.x, (int)pos.y, fontSize, textColor);
}

void ScoreEffect::UpdateStateAndPhysic() {
    Update();
}