#ifndef ENEMY_H
#define ENEMY_H

#include "Object.h"
#include "raylib.h"
#include "GameClock.h" // Thêm để sử dụng GameClock::getInstance()

class Enemy : public Object {
public:
    Enemy(Vector2 pos);
    void Update() override;
    void draw() override;
    void UpdateStateAndPhysic() override;
    void UpdateCollisionProbes() override; // Thêm khai báo override
    void SetDirection(Direction dir) { direction = dir; }
    
protected:
    float maxSpeedX = 50.0f; // Tốc độ tối đa theo chiều ngang
private:
    int textureIndex = 0;
};

class Goomba : public Enemy {
public:
    Goomba(Vector2 pos);
};


#endif