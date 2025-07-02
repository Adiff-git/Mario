#pragma once

#include "Item.h"
#include "ResrcManager.h"

class CourseClearToken : public Item {
private:
    float rotationAngle; 
    float rotationSpeed; 
public:
    CourseClearToken(Vector2 pos);
    ~CourseClearToken() override = default;

    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
    void draw() override;
    void Update() override;
};
