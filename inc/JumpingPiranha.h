#pragma once

#include "Enemy.h"
#include "raylib.h"

class JumpingPiranhaPlant : public Enemy {
private:
    float animationTimer;      
    float animationInterval;    
    int textureIndex;          
    int directionAnim;          
    int updateCounter;         
    int updateThreshold;        

public:
    JumpingPiranhaPlant(Vector2 pos);

    void UpdateStateAndPhysic() override;
};
