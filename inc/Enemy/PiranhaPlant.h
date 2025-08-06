#pragma once
#include "Enemy.h"

class PiranhaPlant : public Enemy {
private:
    float animationTimer;
    float animationInterval;
    bool isMouthOpen;
    
    // Thuộc tính cho chuyển động lên xuống
    Vector2 originalPos;    // Vị trí gốc (vị trí cống)
    float moveTimer;        // Timer để điều khiển chuyển động
    float moveSpeed;        // Tốc độ di chuyển
    float maxHeight;        // Chiều cao tối đa có thể di chuyển lên
    bool isMovingUp;        // Đang di chuyển lên hay xuống
    bool isVisible;         // Có đang hiện ra ngoài cống không

public:
    PiranhaPlant(Vector2 pos);
    void UpdateStateAndPhysic() override;
};