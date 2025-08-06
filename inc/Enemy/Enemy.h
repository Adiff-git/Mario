
#ifndef ENEMY_H
#define ENEMY_H

#include "Object.h"
#include "../inc/SmokeEffect.h"
#include "../inc/ScoreEffect.h"

// Forward declaration
class Character;

class Enemy : public Object {
protected:
    float maxSpeedX;
    int textureIndex;
    int updateCount = 0;

    //animation nhấp nháy
    bool isBlinking;
    float blinkingAcum;        
    float blinkingTime;        
    float blinkingAcumTotal;  
    bool doBlink;              
    bool markedForRemoval;     
    bool hitByFireball;

    // Active/Nonactive state
    bool isActive;
    float activationDistance;    
    
    SmokeEffect* deathSmoke;
    ScoreEffect* scoreEffect;  // Thêm hiệu ứng điểm số
    bool hasDeathEffect;
    float dyingTimer;        // Timer cho trạng thái DYING
    float dyingDuration;   


public:
    Enemy(Vector2 pos, Vector2 size, Vector2 vel, Color color, float friction, int currFrame, Direction dir = DIRECTION_LEFT);
    virtual ~Enemy() = default;
    virtual void UpdateStateAndPhysic() = 0; // Hàm ảo thuần túy
    void Update() override;
    void Update(Vector2 player1Pos, Vector2 player2Pos = {-1000, -1000}); // Overloaded version with both player positions
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
    bool IsHitByFireball() const { return hitByFireball; }
    void SetHitByFireball(bool hit) { hitByFireball = hit; }

    // Active/Nonactive state methods
    bool IsActive() const { return isActive; }
    void SetActive(bool active) { isActive = active; }
    void CheckActivation(Vector2 player1Pos, Vector2 player2Pos = {-1000, -1000});
    float GetDistanceToPlayer(Vector2 playerPos) const;
    float GetDistanceToNearestPlayer(Vector2 player1Pos, Vector2 player2Pos) const; 

    void CreateDeathEffect();
    void UpdateDeathEffect();
    void DrawDeathEffect();
    bool ShouldRemoveDeathEffect() const;
    void UpdateDyingState();
    
    // Score effect methods
    void CreateScoreEffect(int score);
    void UpdateScoreEffect();
    void DrawScoreEffect();
    bool ShouldRemoveScoreEffect() const;
    
    // Static method để truy cập current GameWorld
    static void SetCurrentGameWorld(class GameWorld* world) { currentGameWorld = world; }
    static class GameWorld* GetCurrentGameWorld() { return currentGameWorld; }
    
private:
    static class GameWorld* currentGameWorld; // Static pointer đến GameWorld hiện tại
};

#endif