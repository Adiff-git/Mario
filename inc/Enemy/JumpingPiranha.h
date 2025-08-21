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

    int minY = pos.y - 30;       
    int maxY = pos.y - 100; 
    bool direction = false;
public:
    JumpingPiranhaPlant(Vector2 pos);

    // void Update() override;
    void UpdateStateAndPhysic() override;
};