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
    
    // Determine fire direction based on angle (improved logic for best sprite selection)
    // Positive Y is downward in screen coordinates
    // Choose sprite that requires least rotation
    
    // Normalize degrees to 0-360 range
    while (degrees < 0) degrees += 360;
    while (degrees >= 360) degrees -= 360;
    
    // Choose the sprite that requires the least rotation
    if (degrees >= 315 || degrees <= 45) {
        fireDirection = FireDirection::STRAIGHT; // Right (0°)
    } else if (degrees > 45 && degrees <= 135) {
        fireDirection = FireDirection::DOWN;     // Down (90°) 
    } else if (degrees > 135 && degrees <= 225) {
        fireDirection = FireDirection::STRAIGHT; // Left (180°)
    } else if (degrees > 225 && degrees < 315) {
        fireDirection = FireDirection::UP;
    } else {
        fireDirection = FireDirection::STRAIGHT;
    }
    
    UpdateTexture();
    
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
    Rectangle dest = {pos.x + size.x/2, pos.y + size.y/2, size.x, size.y}; // Center position for rotation
    Vector2 origin = {size.x / 2, size.y / 2}; // Rotate around center
    
    // Calculate actual direction angle from velocity
    float actualAngle = atan2(vel.y, vel.x) * 180.0f / PI;
    
    // Calculate rotation needed based on sprite type and actual direction
    float rotationAngle = CalculateRotationAngle(actualAngle);
    
    DrawTexturePro(*sprite, source, dest, origin, rotationAngle, WHITE);
    
    // Debug info
    std::string debugInfo = "Type: " + std::to_string((int)fireType) + 
                       " Dir: " + std::to_string((int)fireDirection) + 
                       " Angle: " + std::to_string((int)actualAngle) + "°" +
                       " Rot: " + std::to_string((int)rotationAngle) + "°" +
                       " Dist: " + std::to_string((int)distanceLeft);
    DrawText(debugInfo.c_str(), pos.x - 40, pos.y - 15, 8, YELLOW);
}

void BossFireball::UpdateStateAndPhysic() {
    if (isOutOfDistance()) return;
    
    Update();
    
    float dt = GameClock::GetInstance().FIXED_TIME_STEP;
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;
    
    UpdateCollisionProbes();
}

float BossFireball::CalculateRotationAngle(float actualAngle) const {
    // Normalize angle to 0-360 range
    while (actualAngle < 0) actualAngle += 360;
    while (actualAngle >= 360) actualAngle -= 360;
    
    float rotationAngle = 0.0f;
    
    // Calculate rotation needed based on sprite's default orientation
    switch (fireDirection) {
        case FireDirection::STRAIGHT:
            // STRAIGHT sprite points right (0°) by default
            // For left direction, sprite is flipped so we adjust accordingly
            if (horizontalDirection == DIRECTION_RIGHT) {
                rotationAngle = actualAngle; // Direct rotation from 0°
            } else {
                // For left direction, adjust rotation since sprite is horizontally flipped
                rotationAngle = actualAngle - 180.0f;
            }
            break;
            
        case FireDirection::UP:
            // UP sprite points up (-90° or 270°) by default
            rotationAngle = actualAngle - 270.0f;
            break;
            
        case FireDirection::DOWN:
            // DOWN sprite points down (90°) by default  
            rotationAngle = actualAngle - 90.0f;
            break;
    }
    
    // Normalize rotation angle to -180 to 180 range for cleaner rotation
    while (rotationAngle > 180) rotationAngle -= 360;
    while (rotationAngle <= -180) rotationAngle += 360;
    
    return rotationAngle;
}
