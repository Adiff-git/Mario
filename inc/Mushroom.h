
#pragma once
#include "Item.h"
#include "ResrcManager.h"

class Mushroom : public Item {
public:
    Mushroom(Vector2 pos);
    ~Mushroom() override = default;

    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void Draw() override;
    void Update() override;

private:
    bool applyGravity = true;
    bool isMoving;
    float maxSpeedX = 50.0f;
};