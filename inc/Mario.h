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

enum MarioState
{
    SMALL = 0,
    BIG,
    FIRE

};

class Mario : public Object
{
private:
    //mario game information
    int lives;
    int coins;
    int score;
    // mario physics
    float accelerationX;
    float maxSpeedX;
    float SpeedY;
    // mario state
    bool isDucking;
    bool isInvincible = false;
    // const float invincibleFrameTime;
    // float invincibleFrameAcum;
    // int invincibleFrame;
    // float invincibleAcum;
    // const float invincibleTime;

    MarioState marioState;
    MarioState AdditionalState;

    
    std::list<Fireball *> fireballs;

    std::vector<Observer *> observers;

    void Update() override;

public:
    Mario(Vector2 pos, int lives, MarioState form);
    Mario();
    ~Mario() override;

    void AddObserver(Observer *ob);
    void RemoveObserver(Observer *ob);
    void NotifyCoinChange();

    void SetCoins(int c);
    void setInvincible(bool value);
    void SetLives(int lives);
    void SetSprite(Texture2D sprite);
    void SetState(ObjectState state);
    void SetScore(int score);

    void jump();
    void moveLeft();
    void moveRight();
    void stopMoving();
    void Duck();
    void fire();
    void changeToBig();
    void changeToSmall();
    void changetoFire();
    void BeInvincible();
    void Die();
    void Victory();
    void BeHit();

    void SmallToBig();
    void SmallToFire();
    void BigToSmall();
    void BigToFire();


    void Draw() override;
    void HandleInput();

    int GetCoins() const;
    int GetScore() const;
    bool getInvincible() const;
    int GetLives() const;
    MarioState GetMarioState() const;


    void AddLives(int livesToAdd);
    void AddCoins(int coinsToAdd);
    void AddScore(int scoreToAdd);
    bool GetIsDucking() const;
    std::list<Fireball *> *GetFireballs();

    void UpdateCollisionProbes() override;
    void UpdateStateAndPhysic() override;
};