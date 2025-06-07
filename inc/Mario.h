#pragma once
#include <list>

#include "raylib.h"
#include "Drawable.h"
#include "Object.h"
#include "CollisionProbe.h"
#include "GameClock.h"
#include "ResrcManager.h"
#include "Fireball.h"

// Removed unnecessary include of GameWorld.h
// class Fireball; // Forward declaration
enum MarioState {
    SMALL = 0,
    BIG,
    FIRE
};

class Mario : public Object {
    private:
        int lives;
        float accelerationX;
        float maxSpeedX;
        float SpeedY;// for Jumping
        bool isDucking;
        //std::list<Fireball*> fireballs
        MarioState marioState;
        MarioState AdditionalState;

        void Update() override;
        
        std::list<Fireball*>fireballs;
        
    
    public:
        Mario(Vector2 pos, int lives,MarioState form);
        Mario(); // Ensure default constructor exists
        ~Mario() override;

        void jump();
        void moveLeft();
        void moveRight();
        void stopMoving();
        void Duck();
        void changeToBig();
        void changeToSmall();
        void changetoFire();

        void draw() override;
        void HandleInput();

        // Setters
        void SetLives(int lives);
        void SetSprite(Texture2D sprite);
        void SetState(ObjectState state);

        // Getters
        int GetLives() const;
        bool GetIsDucking() const;
        std::list<Fireball*> *GetFireballs();

        void UpdateCollisionProbes() override;
        void UpdateStateAndPhysic() override;

        
};