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
};