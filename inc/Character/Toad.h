#pragma once
#pragma once
#include "Character.h"
#include "../inc/Command/InputHandler.h"
#include "../inc/Character/CharacterType.h"



class Toad : public Character {
private:
   

public:
    Toad(Vector2 pos, int lives, ObjectState form, ControlType controlType)
        : Character(pos, lives, form,  controlType) {
            
        characterType = CharacterType::TOAD;
        SetSpeedY(GetSpeedY() * 1.2f);
        SetSpeedX(GetSpeedX() * 1.2f); // Toad is slightly faster than Mario
        
    }

    CharacterType GetCharType() override {
        return characterType;
    }

    ~Toad() override = default;

    void Update() override;

};