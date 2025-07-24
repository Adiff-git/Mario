#pragma once

#include "Item.h"

class Mario;

class Star : public Item {
private:
    bool applyGravity;
    bool isMoving;
    
    
    bool collected;
    float scoreDisplayTimer;
    float scoreYOffset;
    float scoreAlpha;
    float scoreScale;
    Texture2D* scoreTexture;

public:
    Star(Vector2 pos);

    void Update() override;
    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void Draw() override;
    bool canBeCollected() const;
};