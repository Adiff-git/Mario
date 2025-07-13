#ifndef BANZAI_BILL_H
#define BANZAI_BILL_H

#include "Enemy.h"
#include "ResrcManager.h"

class BanzaiBill : public Enemy {
public:
    BanzaiBill(Vector2 pos);
    void UpdateStateAndPhysic() override;
};

#endif // BANZAI_BILL_H