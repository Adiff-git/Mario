#pragma once
#include <list>
#include "raylib.h"
#include "Drawable.h"
#include "Object.h"
#include "../inc/World/CollisionProbe.h"
#include "../inc/World/GameClock.h"
#include "ResrcManager.h"
#include "Fireball.h"
#include "Observer.h"
#include <vector>
#include "../inc/Command/InputHandler.h"
#include "../inc/SaveData.h"
#include "../inc/Character/CharacterType.h"
class InputHandler;

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

    std::unique_ptr<InputHandler> inputHandler;

    void Update() override;


    CharacterType characterType = CharacterType::NONE;
   

public:
    Character(Vector2 pos, int lives, ObjectState form, ControlType controlType);
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
    void AddScore(int points); 


    void SetDucking(bool ducking) {
        isDucking = ducking;
    }
    void setInputHandler(std::unique_ptr<InputHandler> handler) {
        inputHandler = std::move(handler);
    }
    ControlType getControlType() const {
        return inputHandler ? inputHandler->getControlType() : ControlType::ARROWS;
    }
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
    ObjectState GetAdditionalState() const;

    void SetMarioState(ObjectState state);
    void SetAdditionalState(ObjectState state) {
        AdditionalState = state;
    }
    bool GetIsDucking() const;
    std::list<Fireball *> *GetFireballs();

    void UpdateCollisionProbes() override;
    void UpdateStateAndPhysic() override;

    virtual CharacterType GetCharType() {
        return characterType;
    }


    // virtual PlayerSave ToSave() ;
    // virtual void FromSave(const PlayerSave& s) ;
};