#pragma once

#include "Item.h"
#include "ResrcManager.h"

class ThreeUpMoon : public Item {
public:
    ThreeUpMoon(Vector2 pos);
    ~ThreeUpMoon() override = default;

    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void Draw() override;
    void Update() override;
    bool canBeCollected() const override { return !collected; } 

private:
    bool applyGravity = true;
    
    bool collected = false;
    float scoreDisplayTimer = 0.0f; 
    float scoreYOffset = 0.0f;      
    float scoreAlpha = 1.0f;        
    float scoreScale = 1.0f;
    Texture2D* scoreTexture = nullptr;
};