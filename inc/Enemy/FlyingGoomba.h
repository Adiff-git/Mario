#ifndef FLYING_GOOMBA_H
#define FLYING_GOOMBA_H
#include "Enemy.h"

class FlyingGoomba : public Enemy
{
public:
    FlyingGoomba(Vector2 pos);
    void UpdateStateAndPhysic() override;

private:
    int updateCount = 0;
    int textureIndex = 0;
    float flyingCycleTime = 0.0f;
    float flyingCycleDuration = 2.0f;
    float flyingAmplitude = 20.0f;
    float baseY = pos.y;

    float startX = 0.0f;
    float moveDistance = 100.0f;
    bool movingLeft = true;
};
#endif // FLYING_GOOMBA_H