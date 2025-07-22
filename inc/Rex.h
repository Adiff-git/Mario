#ifndef REX_H_
#define REX_H_

#include "Enemy.h"
#include "ResrcManager.h"

class Rex : public Enemy {
public:
    Rex(Vector2 pos);
    void UpdateStateAndPhysic() override;
    void OnHit(); 
    int GetHitCount() const { return hitCount; } 

private:
    int jumpCooldown; 
    int hitCount;
    Vector2 originalSize; 
    int animationFrame; 
    int updateCount = 0;
};

#endif 