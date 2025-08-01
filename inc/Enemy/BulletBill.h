#ifndef BULLET_BILL_H
#define BULLET_BILL_H

#include "Enemy.h"
#include "ResrcManager.h"

class BulletBill : public Enemy {
public:
    BulletBill(Vector2 pos);
    void UpdateStateAndPhysic() override;
};

#endif