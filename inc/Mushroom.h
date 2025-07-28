#pragma once

#include "Item.h"

class Mushroom : public Item {
private:
    bool applyGravity;
    bool isMoving;
    float maxSpeedX = 50.0f; 
    Texture2D* sprite;
    
    bool blinking;
    float blinkingAcum;
    float blinkingTime;
    bool doBlink;
    float blinkingAcumTotal;

public:
    Mushroom(Vector2 pos);

    void updateMario(Character& mario) override;
    void playCollisionSound() override;
    void Update() override;
    void Draw() override;

    void StartMoving();//move 
};