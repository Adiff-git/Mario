#ifndef REDKOOPA_H_
#define REDKOOPA_H_

#include "Enemy.h"
#include "ResrcManager.h"

class RedKoopa : public Enemy {
public:
    RedKoopa(Vector2 pos);
    void UpdateStateAndPhysic() override;
    void OnHit(bool fromLeft); 
    int GetHitCount() const { return hitCount; } 
    bool IsMoving() const { return isMoving; }
    bool isMoving;

private:
    int jumpCooldown; 
    int hitCount; 
    Vector2 originalSize;  
    void EnterShell();
    void EnterShellWithVelocity(float velX);
};

#endif