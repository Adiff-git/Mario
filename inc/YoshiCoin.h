#pragma once

#include "Item.h"
#include "ResrcManager.h"

class YoshiCoin : public Item {
private:
    Texture2D* scoreTexture;
    bool collected;
    float scoreDisplayTimer; 
    float scoreYOffset;      // Vị trí Y của điểm số (di chuyển lên trên)
    float scoreAlpha;        // Độ trong suốt của điểm số
    float scoreScale;        // Kích thước của điểm số 
public:
    YoshiCoin(Vector2 pos);
    ~YoshiCoin() override = default;

    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void Draw() override;
    void Update() override;
    bool canBeCollected() const override { return !collected; } 
};