#pragma once

#include "Item.h"
#include "raylib.h"

class Mario;

class OneUpMushroom : public Item {
private:
    bool applyGravity;
    bool isMoving;
    float maxSpeedX = 50.0f; 

    // Hiệu ứng xuất hiện "1UP"
    bool collected;
    float scoreDisplayTimer; 
    float scoreYOffset;      
    float scoreAlpha;        
    float scoreScale;
    Texture2D* scoreTexture; 

public:
    OneUpMushroom(Vector2 pos);

    void StartMoving();

    void Update(); // Không truyền world

    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void Draw() override;
    bool canBeCollected() const override { return !collected; } 
};