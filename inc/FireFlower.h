#pragma once
#include "Item.h"
#include "ResrcManager.h"

class FireFlower : public Item
{
private:
    bool blinking;
    float blinkingAcum;
    float blinkingTime;
    bool doBlink;
    float blinkingAcumTotal;

    bool collected;
    float scoreDisplayTimer; 
    float scoreYOffset;      
    float scoreAlpha;        
    float scoreScale;  
    float frameAcumulator;   // Cho animation mượt

    Texture2D* scoreTexture;

public:
    FireFlower(Vector2 pos);
    ~FireFlower() override = default;

    void updateMario(Mario &mario) override;
    void playCollisionSound() override;
    void Draw() override;
    void NewFunction(Texture2D &tex, std::string &texKey, bool &retFlag);
    void Update() override;
    bool canBeCollected() const override { return !collected && !blinking; } 
};