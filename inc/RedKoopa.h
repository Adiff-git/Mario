#ifndef RED_KOOPA_H
#define RED_KOOPA_H

#include "Enemy.h"
#include "ResrcManager.h"

class RedKoopa : public Enemy {
public:
    RedKoopa(Vector2 pos);
    void UpdateStateAndPhysic() override;
};

#endif