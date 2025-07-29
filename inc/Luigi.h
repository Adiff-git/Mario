#pragma once
#include "Character.h"


class Luigi : public Character {
private:
   

public:
    Luigi(Vector2 pos, int lives, ObjectState form)
        : Character(pos, lives, form) {
        
    }

    

    ~Luigi() override = default;

    void Update() override;

};