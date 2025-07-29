#pragma once
#include "Character.h"
#include "InputHandler.h"


class Luigi : public Character {
private:
   

public:
    Luigi(Vector2 pos, int lives, ObjectState form, ControlType controlType)
        : Character(pos, lives, form,  controlType) {
        
    }

    

    ~Luigi() override = default;

    void Update() override;

};