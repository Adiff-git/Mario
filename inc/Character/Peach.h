#pragma once
#include "Character.h"
#include "../inc/Command/InputHandler.h"


class Peach : public Character {
private:
   

public:
    Peach(Vector2 pos, int lives, ObjectState form, ControlType controlType)
        : Character(pos, lives, form,  controlType) {
        
    }

    

    ~Peach() override = default;

    void Update() override;

};