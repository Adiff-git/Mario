#ifndef YELLOW_KOOPA_H
#define YELLOW_KOOPA_H

#include "Enemy.h"
#include "ResrcManager.h"

class YellowKoopa : public Enemy {
public:
    YellowKoopa(Vector2 pos);
    void UpdateStateAndPhysic() override;
};

#endif