#pragma once

#include "Item.h"
#include "raylib.h"

class Mario;

class OneUpMushroom : public Item {
private:
    bool applyGravity;
    bool isMoving;
    float maxSpeedX = 50.0f; 

public:
    OneUpMushroom(Vector2 pos);

    void StartMoving();

    void Update(); // Không truyền world

    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void Draw() override;
};
