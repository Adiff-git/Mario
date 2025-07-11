#pragma once

#include "Item.h"

class Mario;

class Start : public Item {
private:
    bool applyGravity;
    bool isMoving;

public:
    Start(Vector2 pos);

    void Update() override;
    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void draw() override;
};
