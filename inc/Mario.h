#pragma once
#include "Character.h"


class Mario : public Character {
private:
   

public:
    Mario(Vector2 pos, int lives, ObjectState form)
        : Character(pos, lives, form) {
        
    }

    

    ~Mario() override = default;

    void Update() override;

};