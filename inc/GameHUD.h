#pragma once

#include "Drawable.h"
#include "Character.h"

class GameHUD : public Drawable {
    private:

        float timeLeft = 180;
        Character* player1;
        Character* player2;

        // player 1
        Vector2 coinsPosition1;
        Vector2 coinsSize1;
        Texture2D *coinTexture1;

        Vector2 scorePosition1;
        Vector2 scoreSize1;
        Texture2D *MarioHUD1;

        Vector2 Player1StatePos;
        Vector2 Player1StateSize;
        Texture2D *Player1StateTexture;

        Vector2 block1;
        Vector2 block1Size;
        Texture2D *block1Texture;
        

         // player 2
        Vector2 coinsPosition2;
        Vector2 coinsSize2;
        Texture2D *coinTexture2;

        Vector2 scorePosition2;
        Vector2 scoreSize2;
        Texture2D *MarioHUD2;

        Vector2 Player2StatePos;
        Vector2 Player2StateSize;
        Texture2D *Player2StateTexture;

        Vector2 block2;
        Vector2 block2Size;
        Texture2D *block2Texture;
        
        
        Font* font;
    public:
        GameHUD() = default;
        GameHUD(Character* mario);
        GameHUD(Character* mario, Character* luigi);// for multiple player

        void Draw() override;
        void setFont(Font& font);
        void setTextColor(Color color);
        void Update(float deltaTime);
        float GetTimeLeft() const {
            return timeLeft;
        }
        void Reset() {
            timeLeft = 180;
        }
};