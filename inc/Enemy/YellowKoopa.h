#ifndef YELLOW_KOOPA_H
#define YELLOW_KOOPA_H

#include "Enemy.h"
#include "ResrcManager.h"

class YellowKoopa : public Enemy {
private:
    int jumpCooldown;
    int hitCount;
    int updateCount;
    int textureIndex;
    Vector2 originalSize;

public:
    YellowKoopa(Vector2 pos);
    void UpdateStateAndPhysic() override;
    void EnterShell();
    void EnterShellWithVelocity(float velX);
    void OnHit(bool fromLeft);
    int GetHitCount() const { return hitCount; }
    bool IsMoving() const { return isMoving; }
    bool isMoving;
};

#endif