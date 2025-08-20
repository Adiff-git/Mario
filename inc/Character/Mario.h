#pragma once
#include "Character.h"
#include "../inc/Command/InputHandler.h"
#include "../inc/Character/CharacterType.h"

class Mario : public Character {
private:
   

public:
    Mario(Vector2 pos, int lives, ObjectState form, ControlType controlType)
        : Character(pos, lives, form, controlType) {
            
        characterType = CharacterType::MARIO;
        SetSpeedY(GetSpeedY() * 0.95f);
        
    }

    CharacterType GetCharType() override {
        return characterType;
    }
    

    ~Mario() override = default;

    void Update() override;

};