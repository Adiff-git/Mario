
#ifndef ENEMY_H
#define ENEMY_H

#include "Object.h"
class Enemy : public Object {
protected:
    float maxSpeedX;
    int textureIndex;

    //animation nhấp nháy
    bool isBlinking;
    float blinkingAcum;        
    float blinkingTime;        
    float blinkingAcumTotal;  
    bool doBlink;              
    bool markedForRemoval;     

public:
    Enemy(Vector2 pos, Vector2 size, Vector2 vel, Color color, float friction, int currFrame, Direction dir);
    virtual ~Enemy() = default;
    virtual void UpdateStateAndPhysic() = 0; // Hàm ảo thuần túy
    void Update() override;
    void UpdateCollisionProbes() override;
    void drawCollisionProbes();
    void SetDirection(Direction dir) { direction = dir; }
    Direction GetDirection() const { return direction; }
    void Draw() override ;

    void StartBlinking(float duration = 1.0f, float interval = 0.1f);
    void UpdateBlinking();
    bool IsBlinking() const;
    void StopBlinking();
    bool ShouldRender() const;
    bool ShouldBeRemoved() const; 
};

#endif