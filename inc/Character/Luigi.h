#pragma once
#include "Character.h"
#include "../inc/Character/CharacterType.h"
#include "../inc/Command/InputHandler.h"



class Luigi : public Character {
private:
   

public:
    Luigi(Vector2 pos, int lives, ObjectState form, ControlType controlType)
        : Character(pos, lives, form,  controlType) {
            
        characterType = CharacterType::LUIGI;
        
    }

    
    CharacterType GetCharType() override {
        return characterType;
    }
    ~Luigi() override = default;

    void Update() override;

};