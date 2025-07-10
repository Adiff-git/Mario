#pragma once
#include "Enemy.h"

class PiranhaPlant : public Enemy {
private:
    float animationTimer;
    float animationInterval;
    bool isMouthOpen;

public:
    PiranhaPlant(Vector2 pos);
    void UpdateStateAndPhysic() override;
};
