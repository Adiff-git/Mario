#pragma once

#include "Enemy.h"
#include "raylib.h"

class JumpingPiranhaPlant : public Enemy
{
private:
    float animationTimer;
    float animationInterval;
    int textureIndex;
    int directionAnim;
    int updateCounter;
    int updateThreshold;

    int minY = pos.y + 30;       // starting (bottom) position
    int maxY = pos.y - 100; // peak (top) position — adjust as needed
    bool direction = false;
public:
    JumpingPiranhaPlant(Vector2 pos);

    // void Update() override;
    void UpdateStateAndPhysic() override;
};
