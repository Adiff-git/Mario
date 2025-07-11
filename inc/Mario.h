#pragma once
#include <list>

#include "raylib.h"
#include "Drawable.h"
#include "Object.h"
#include "CollisionProbe.h"
#include "GameClock.h"
#include "ResrcManager.h"
#include "Fireball.h"
#include "Observer.h" 
#include <vector>
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
        int coins;
        float accelerationX;
        float maxSpeedX;
        float SpeedY;// for Jumping
        bool isDucking;
        bool isInvincible = false; // Added invincibility state
        MarioState marioState;
        MarioState AdditionalState;

        void Update() override;
        std::list<Fireball*> fireballs;
        std::vector<Observer *> observers;

    public:
        Mario(Vector2 pos, int lives,MarioState form);
        Mario(); // Ensure default constructor exists
        ~Mario() override;

        void AddObserver(Observer *ob);
        void RemoveObserver(Observer *ob);
        void NotifyCoinChange();

        void jump();
        void moveLeft();
        void moveRight();
        void stopMoving();
        void Duck();
        void fire();
        void changeToBig();
        void changeToSmall();
        void changetoFire();

        void draw() override;
        void HandleInput();

        // Setters
        void SetLives(int lives);
        void SetSprite(Texture2D sprite);
        void SetState(ObjectState state);
        void SetCoins(int c);
        void setInvincible(bool value);

        // Getters
        int GetCoins() const;
        bool getInvincible() const;
        int GetLives() const;
        bool GetIsDucking() const;
        std::list<Fireball*> *GetFireballs();

        void UpdateCollisionProbes() override;
        void UpdateStateAndPhysic() override;

        
};