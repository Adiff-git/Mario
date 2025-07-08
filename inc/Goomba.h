#ifndef GOOMBA_H_
#define GOOMBA_H_

#include "Enemy.h"
#include "ResrcManager.h"

class Goomba : public Enemy {
public:
    Goomba(Vector2 pos);
    void UpdateStateAndPhysic() override;
};

#endif // GOOMBA_H_