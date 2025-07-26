#pragma once
#include <list>
#include "PlayerState.h"
#include "raylib.h"
#include "Drawable.h"
#include "Object.h"
#include "CollisionProbe.h"
#include "GameClock.h"
#include "ResrcManager.h"
#include "Fireball.h"
#include "Observer.h"
#include <vector>

class Luigi : public Object
{
private:
    //Luigi game information
    int lives;
    int coins;
    int score;
    // Luigi physics
    float accelerationX;
    float maxSpeedX;
    float SpeedY;
    // Luigi state
    bool isDucking;
    bool isInvincible = false;
    float invincibleTimer;
    float invincibleDuration;
    bool blinking;
    bool doBlink;
    float blinkingAcum;
    float blinkingTime;
    float blinkingAcumTotal;

    PlayerState LuigiState;
    
    std::list<Fireball *> fireballs;

    std::vector<Observer *> observers;

    void Update() override;

public:
    Luigi(Vector2 pos, int lives, PlayerState LuigiState);
    Luigi();
    ~Luigi() override;

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

    int GetCoins() const;
    int GetScore() const;
    bool getInvincible() const;
    int GetLives() const;
    PlayerState GetPlayerState() const;

    void SetPlayerState(PlayerState state);
    bool GetIsDucking() const;
    std::list<Fireball *> *GetFireballs();

    void UpdateCollisionProbes() override;
    void UpdateStateAndPhysic() override;
};