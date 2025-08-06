#pragma once
#pragma once
#include "Character.h"
#include "../inc/Command/InputHandler.h"


class Toad : public Character {
private:
   

public:
    Toad(Vector2 pos, int lives, ObjectState form, ControlType controlType)
        : Character(pos, lives, form,  controlType) {
        
    }

    

    ~Toad() override = default;

    void Update() override;

};