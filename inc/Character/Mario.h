#pragma once
#include "Character.h"
#include "../inc/Command/InputHandler.h"

class Mario : public Character {
private:
   

public:
    Mario(Vector2 pos, int lives, ObjectState form, ControlType controlType)
        : Character(pos, lives, form, controlType) {
        
    }

    

    ~Mario() override = default;

    void Update() override;

};