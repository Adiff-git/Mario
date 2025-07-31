#pragma once

#include "Drawable.h"
#include "Character.h"

class GameHUD : public Drawable {
    private:
        Character* player1;
        Character* player2;

        Vector2 coinsPosition;
        Vector2 coinsSize;
        Texture2D *coinTexture;

        Vector2 scorePosition;
        Vector2 scoreSize;
        Texture2D *MarioHUD;

        Vector2 marioStatePosition;
        Vector2 marioStateSize;
        Texture2D *marioStateTexture;

        Vector2 block;
        Vector2 blockSize;
        Texture2D *blockTexture;

        
        Font* font;
    public:
        GameHUD() = default;
        GameHUD(Character* mario);
        GameHUD(Character* mario, Character* luigi);// for multiple player

        void Draw() override;
        void setFont(Font& font);
        void setTextColor(Color color);
};