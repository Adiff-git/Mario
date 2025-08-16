#pragma once
#include "Character.h"
#include "../inc/Command/InputHandler.h"
#include "../inc/Character/CharacterType.h"


class Peach : public Character {
private:
   

public:
    Peach(Vector2 pos, int lives, ObjectState form, ControlType controlType)
        : Character(pos, lives, form,  controlType) {
            
        characterType = CharacterType::PEACH;
        SetSpeedY(GetSpeedY() * 0.9f); // Peach is slightly faster than Mario
        SetSpeedX(GetSpeedX() * 1.2f); // Peach is slightly faster than Mario
        
    }

    CharacterType GetCharType() override {
        return characterType;
    }
    

    ~Peach() override = default;

    void Update() override;

};