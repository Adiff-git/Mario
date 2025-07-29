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



class Character : public Object
{
protected:
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
    float invincibleTimer;
    float invincibleDuration;
    bool blinking;
    bool doBlink;
    float blinkingAcum;
    float blinkingTime;
    float blinkingAcumTotal;

    ObjectState CharState;
    ObjectState AdditionalState;

    
    std::list<Fireball *> fireballs;

    std::vector<Observer *> observers;

    void Update() override;

public:
    Character(Vector2 pos, int lives, ObjectState form);
    Character();
    ~Character() override;

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




    void Draw() override;
    void HandleInput();
    // Removed redundant declaration of Update()

    int GetCoins() const;
    int GetScore() const;
    bool getInvincible() const;
    int GetLives() const;
    ObjectState GetMarioState() const;

    void SetMarioState(ObjectState state);
    bool GetIsDucking() const;
    std::list<Fireball *> *GetFireballs();

    void UpdateCollisionProbes() override;
    void UpdateStateAndPhysic() override;
};