#pragma once
#include "Object.h"
#include "ResrcManager.h"
#include "../inc/World/GameClock.h"

enum class FireType {
    SMALL,
    NORMAL,
    BIG
};

enum class FireDirection {
    UP,
    STRAIGHT,
    DOWN
};

class BossFireball : public Object {
private:
    Texture2D* sprite;
    static constexpr float MAX_DISTANCE = 600.0f;
    float distanceLeft;
    static constexpr float FIREBALL_SPEED = 300.0f;
    
    FireType fireType;
    FireDirection fireDirection;
    Direction horizontalDirection;
    
    void Update() override;
    void UpdateTexture();
    float CalculateRotationAngle(float actualAngle) const; // Calculate rotation for sprite based on actual direction
    
public:
    BossFireball(Vector2 pos, Vector2 direction, FireType type = FireType::NORMAL);
    bool isOutOfDistance() const;
    void Draw() override;
    void UpdateStateAndPhysic() override;
    
    // Getters
    FireType getFireType() const { return fireType; }
    FireDirection getFireDirection() const { return fireDirection; }
};
