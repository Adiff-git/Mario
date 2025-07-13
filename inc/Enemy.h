
#ifndef ENEMY_H
#define ENEMY_H

#include "Object.h"

enum EnemyName {
    GOOMBA,
    // ... continue

};

class Enemy : public Object {
protected:
    float maxSpeedX;
    int textureIndex;

public:
    Enemy(Vector2 pos, Vector2 size, Vector2 vel, Color color, float friction, int currFrame, Direction dir);
    virtual ~Enemy() = default;
    virtual void UpdateStateAndPhysic() = 0; // Hàm ảo thuần túy
    
    void Update() override;
    void draw() override;
    void UpdateCollisionProbes() override;
    void drawCollisionProbes();
    void SetDirection(Direction dir) { direction = dir; }
    Direction GetDirection() const { return direction; }
};

#endif