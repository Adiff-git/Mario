#include "BossFireball.h"
#include <cmath>
#include <iostream>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

BossFireball::BossFireball(Vector2 pos, Vector2 direction, FireType type) 
    : Object(pos, Vector2{32, 32}, WHITE), 
      distanceLeft(MAX_DISTANCE), 
      fireType(type), 
      fireDirection(FireDirection::STRAIGHT),
      horizontalDirection(DIRECTION_RIGHT) {
    
    // Normalize direction vector
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    // Set velocity
    vel.x = direction.x * FIREBALL_SPEED;
    vel.y = direction.y * FIREBALL_SPEED;
    
    // Determine fire direction based on velocity angle
    float angle = atan2(direction.y, direction.x);
    float degrees = angle * 180.0f / PI;
    
    // Determine horizontal direction
    horizontalDirection = (direction.x >= 0) ? DIRECTION_RIGHT : DIRECTION_LEFT;
    
    // Determine fire direction based on angle (corrected logic)
    // Positive Y is downward in screen coordinates
    if (degrees > 30 && degrees < 150) {
        fireDirection = FireDirection::DOWN;  // Shooting downward (positive Y)
    } else if (degrees < -30 && degrees > -150) {
        fireDirection = FireDirection::UP;    // Shooting upward (negative Y)
    } else {
        fireDirection = FireDirection::STRAIGHT; // Shooting horizontally
    }
    
    // std::cout << "BossFireball created: angle=" << degrees << "°, fireDirection=" << (int)fireDirection 
    //           << ", horizontalDirection=" << (horizontalDirection == DIRECTION_RIGHT ? "RIGHT" : "LEFT") << std::endl;
    
    std::cout << "BossFireball created: angle=" << degrees << "°, fireDirection=" << (int)fireDirection 
              << ", horizontalDirection=" << (horizontalDirection == DIRECTION_RIGHT ? "RIGHT" : "LEFT") << std::endl;
    
    // Set initial texture
    UpdateTexture();
    
    // Set up collision probes
    cpN.setColor(ORANGE);
    cpS.setColor(ORANGE);
    cpE.setColor(ORANGE);
    cpW.setColor(ORANGE);
    cpN.setSize(Vector2{size.x-4, 2});
    cpS.setSize(Vector2{size.x-4, 2});
    cpE.setSize(Vector2{2, size.y-4});
    cpW.setSize(Vector2{2, size.y-4});
    UpdateCollisionProbes();
    
    frameTime = 0.15f;
    frameAcumulator = 0.0f;
    currentFrame = 0;
    maxFrames = 3;
}

bool BossFireball::isOutOfDistance() const {
    return distanceLeft <= 0.0f;
}

void BossFireball::UpdateTexture() {
    try {
        ResrcManager& resrc = ResrcManager::GetInstance();
        
        std::string textureName = "Fire_";
        
        // Add fire type
        switch (fireType) {
            case FireType::SMALL:
                textureName += "Small_";
                break;
            case FireType::NORMAL:
                textureName += "Normal_";
                break;
            case FireType::BIG:
                textureName += "Big_";
                break;
        }
        
        // Add fire direction
        switch (fireDirection) {
            case FireDirection::UP:
                textureName += "UP_";
                break;
            case FireDirection::STRAIGHT:
                textureName += "STRAIGHT_";
                break;
            case FireDirection::DOWN:
                textureName += "DOWN_";
                break;
        }
        
        // Add horizontal direction
        if (horizontalDirection == DIRECTION_LEFT) {
            textureName += "Left";
        } else {
            textureName += "Right";
        }
        
        sprite = &resrc.getTexture(textureName);
        
    } catch (...) {
        // Fallback to a default texture if loading fails
        sprite = nullptr;
        std::cout << "Failed to load BossFireball texture" << std::endl;
    }
}

void BossFireball::Update() {
    if (isOutOfDistance()) return;
    
    float dt = GameClock::GetInstance().FIXED_TIME_STEP;
    
    // Update frame animation
    frameAcumulator += dt;
    if (frameAcumulator >= frameTime) {
        frameAcumulator = 0.0f;
        currentFrame = (currentFrame + 1) % maxFrames;
    }
    
    // Update distance
    float distanceTraveled = sqrt(vel.x * vel.x + vel.y * vel.y) * dt;
    distanceLeft -= distanceTraveled;
}

void BossFireball::Draw() {
    if (isOutOfDistance() || !sprite) return;
    
    Rectangle source = {0, 0, (float)sprite->width, (float)sprite->height};
    Rectangle dest = {pos.x, pos.y, size.x, size.y};
    Vector2 origin = {0, 0};
    
    DrawTexturePro(*sprite, source, dest, origin, 0.0f, WHITE);
    
    // Debug info
    std::string debugInfo = "Type: " + std::to_string((int)fireType) + 
                       " Dir: " + std::to_string((int)fireDirection) + 
                       " Dist: " + std::to_string((int)distanceLeft);
    DrawText(debugInfo.c_str(), pos.x, pos.y - 15, 8, YELLOW);
}

void BossFireball::UpdateStateAndPhysic() {
    if (isOutOfDistance()) return;
    
    Update();
    
    float dt = GameClock::GetInstance().FIXED_TIME_STEP;
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
    
    UpdateCollisionProbes();
}
