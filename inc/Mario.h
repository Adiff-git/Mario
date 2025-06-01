#pragma once

#include "raylib.h"
#include "Drawable.h"
#include "Object.h"
#include "CollisionProbe.h"
#include "GameClock.h"
#include "ResrcManager.h"
#include <list>
enum MarioState {
    SMALL = 0,
    BIG,
};

class Mario : public Object {
    private:
        int lives;
        float accelerationX;
        float maxSpeedX;
        float SpeedY;// for Jumping
       
        MarioState marioState;
        MarioState AdditionalState;

        void Update() override;
        void UpdateCollisionProbes() override;
        void UpdateStateAndPhysic() override;
    
    public:
        Mario(Vector2 pos, int lives,MarioState form);
        Mario();
        ~Mario() override;

        void jump();
        void moveLeft();
        void moveRight();
        void stopMoving();

        void Duck();

        void changeToBig();
        void changeToSmall();

        void draw() override;


        // Setters
        void SetLives(int lives);
        void SetSprite(Texture2D sprite);
        void SetState(MarioState state);

        // Getters
        int GetLives() const;

        void HandleInput();
};