#ifndef BOB_OMB_H
#define BOB_OMB_H

#include "Enemy.h"
#include "ResrcManager.h"

class Bob_omb : public Enemy {
public:
    Bob_omb(Vector2 pos);
    void UpdateStateAndPhysic() override;
private:
    float explosionTimer;
    bool isActivated;
};

#endif