#ifndef GREEN_KOOPA_H
#define GREEN_KOOPA_H

#include "Enemy.h"
#include "ResrcManager.h"

class GreenKoopa : public Enemy {
public:
    GreenKoopa(Vector2 pos);
    void UpdateStateAndPhysic() override;
};

#endif