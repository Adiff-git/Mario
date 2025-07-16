#pragma once

#include "Item.h"

class Mario;

class Star : public Item {
private:
    bool applyGravity = true;
    bool isMoving;

public:
    Star(Vector2 pos);

    void Update() override;
    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void Draw() override;
};