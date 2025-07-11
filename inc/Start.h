#pragma once

#include "Item.h"
#include "ResrcManager.h"

class Start : public Item {
public:
    Start(Vector2 pos);
    ~Start() override = default;

    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void draw() override;
    void Update() override;
};