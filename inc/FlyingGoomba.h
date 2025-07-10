
#include "Enemy.h"

class FlyingGoomba : public Enemy {
public:
    FlyingGoomba(Vector2 pos);
    void UpdateStateAndPhysic() override;

private:
    int textureIndex = 0;
};


