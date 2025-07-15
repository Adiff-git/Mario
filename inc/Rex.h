#ifndef REX_H_
#define REX_H_

#include "Enemy.h"
#include "ResrcManager.h"

class Rex : public Enemy {
public:
    Rex(Vector2 pos);
    void UpdateStateAndPhysic() override;
    void OnHit(); // Hàm xử lý khi bị đạp
    int GetHitCount() const { return hitCount; } // Lấy số lần bị đạp

private:
    int jumpCooldown; // Biến để theo dõi thời gian cooldown nhảy
    int hitCount; // Biến để theo dõi số lần bị đạp
    Vector2 originalSize; // Lưu kích thước va chạm ban đầu
    int animationFrame; // Biến để theo dõi frame trong animation
};

#endif // REX_H_