#pragma once
#include "Enemy.h"

class JumpingPiranhaPlant : public Enemy {
private:
    float animationTimer;
    float animationInterval;
    int textureIndex;
    int directionAnim; 

public:
    JumpingPiranhaPlant(Vector2 pos);
    void UpdateStateAndPhysic() override;
};
