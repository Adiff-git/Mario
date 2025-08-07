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
        
    }

    CharacterType GetCharType() override {
        return characterType;
    }

    ~Toad() override = default;

    void Update() override;

};