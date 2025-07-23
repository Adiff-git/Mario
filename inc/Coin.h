#pragma once

#include "Item.h"
#include <vector>
#include <string>

class Coin : public Item {
private:
    std::vector<Texture2D*> coinFrames;
    Texture2D* scoreTexture;   
    bool collected;
    Vector2 vel;
    Vector2 origin;
    float riseAmount;      
    float maxRise;         
    bool showScore;   


public:
    Coin(Vector2 pos);
    ~Coin() override = default;

    void Update() override;
    void Draw() override;
    void updateMario(Mario& mario) override;
    void playCollisionSound() override;
};